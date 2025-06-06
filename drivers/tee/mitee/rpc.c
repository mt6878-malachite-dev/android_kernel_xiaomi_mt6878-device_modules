// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015-2021, Linaro Limited
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/slab.h>

#include <tee_drv.h>
#include "optee_private.h"
#include "optee_rpc_cmd.h"
#include "optee_msg.h"
#include "optee_bench.h"
#include "rpc_callback.h"

struct wq_entry {
	struct list_head link;
	struct completion c;
	u32 key;
};

struct callback_item {
	struct list_head link;
	uint32_t (*callback)(struct optee_msg_param_value *value, void *buf, uint32_t size_in, uint32_t *size_out);
	uint32_t module_id;
	uint32_t cmd;
};

void optee_wait_queue_init(struct optee_wait_queue *priv)
{
	mutex_init(&priv->mu);
	INIT_LIST_HEAD(&priv->db);
}

void optee_wait_queue_exit(struct optee_wait_queue *priv)
{
	mutex_destroy(&priv->mu);
}

void mitee_rpc_callback_queue_init(struct mitee_rpc_callback_queue *queue)
{
	mutex_init(&queue->mut);
	INIT_LIST_HEAD(&queue->item);
}

void mitee_rpc_callback_queue_deinit(struct mitee_rpc_callback_queue *queue)
{
	mutex_destroy(&queue->mut);
}

static void handle_rpc_func_cmd_get_time(struct optee_msg_arg *arg)
{
	struct timespec64 ts;

	if (arg->num_params != 1)
		goto bad;
	if ((arg->params[0].attr & OPTEE_MSG_ATTR_TYPE_MASK) !=
	    OPTEE_MSG_ATTR_TYPE_VALUE_OUTPUT)
		goto bad;

	ktime_get_real_ts64(&ts);
	arg->params[0].u.value.a = ts.tv_sec;
	arg->params[0].u.value.b = ts.tv_nsec;

	arg->ret = TEEC_SUCCESS;
	return;
bad:
	arg->ret = TEEC_ERROR_BAD_PARAMETERS;
}

#if IS_REACHABLE(CONFIG_I2C)
static void handle_rpc_func_cmd_i2c_transfer(struct tee_context *ctx,
					     struct optee_msg_arg *arg)
{
	struct optee *optee = tee_get_drvdata(ctx->teedev);
	struct tee_param *params;
	struct i2c_adapter *adapter;
	struct i2c_msg msg = {};
	size_t i;
	int ret = -EOPNOTSUPP;
	u8 attr[] = {
		TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT,
		TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INPUT,
		TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT,
		TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_OUTPUT,
	};

	if (arg->num_params != ARRAY_SIZE(attr)) {
		arg->ret = TEEC_ERROR_BAD_PARAMETERS;
		return;
	}

	params = kmalloc_array(arg->num_params, sizeof(struct tee_param),
			       GFP_KERNEL);
	if (!params) {
		arg->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return;
	}

	if (optee->ops->from_msg_param(optee, params, arg->num_params,
				       arg->params))
		goto bad;

	for (i = 0; i < arg->num_params; i++) {
		if (params[i].attr != attr[i])
			goto bad;
	}

	adapter = i2c_get_adapter(params[0].u.value.b);
	if (!adapter)
		goto bad;

	if (params[1].u.value.a & OPTEE_RPC_I2C_FLAGS_TEN_BIT) {
		if (!i2c_check_functionality(adapter, I2C_FUNC_10BIT_ADDR)) {
			i2c_put_adapter(adapter);
			goto bad;
		}

		msg.flags = I2C_M_TEN;
	}

	msg.addr = params[0].u.value.c;
	msg.buf = params[2].u.memref.shm->kaddr;
	msg.len = params[2].u.memref.size;

	switch (params[0].u.value.a) {
	case OPTEE_RPC_I2C_TRANSFER_RD:
		msg.flags |= I2C_M_RD;
		break;
	case OPTEE_RPC_I2C_TRANSFER_WR:
		break;
	default:
		i2c_put_adapter(adapter);
		goto bad;
	}

	ret = i2c_transfer(adapter, &msg, 1);

	if (ret < 0) {
		arg->ret = TEEC_ERROR_COMMUNICATION;
	} else {
		params[3].u.value.a = msg.len;
		if (optee->ops->to_msg_param(optee, arg->params,
					     arg->num_params, params))
			arg->ret = TEEC_ERROR_BAD_PARAMETERS;
		else
			arg->ret = TEEC_SUCCESS;
	}

	i2c_put_adapter(adapter);
	kfree(params);
	return;
bad:
	kfree(params);
	arg->ret = TEEC_ERROR_BAD_PARAMETERS;
}
#else
static void handle_rpc_func_cmd_i2c_transfer(struct tee_context *ctx,
					     struct optee_msg_arg *arg)
{
	arg->ret = TEEC_ERROR_NOT_SUPPORTED;
}
#endif

static struct wq_entry *wq_entry_get(struct optee_wait_queue *wq, u32 key)
{
	struct wq_entry *w;

	mutex_lock(&wq->mu);

	list_for_each_entry (w, &wq->db, link)
		if (w->key == key)
			goto out;

	w = kmalloc(sizeof(*w), GFP_KERNEL);
	if (w) {
		init_completion(&w->c);
		w->key = key;
		list_add_tail(&w->link, &wq->db);
	}
out:
	mutex_unlock(&wq->mu);
	return w;
}

static void wq_sleep(struct optee_wait_queue *wq, u32 key)
{
	struct wq_entry *w = wq_entry_get(wq, key);

	if (w) {
		wait_for_completion(&w->c);
		mutex_lock(&wq->mu);
		list_del(&w->link);
		mutex_unlock(&wq->mu);
		kfree(w);
	}
}

static void wq_wakeup(struct optee_wait_queue *wq, u32 key)
{
	struct wq_entry *w = wq_entry_get(wq, key);

	if (w)
		complete(&w->c);
}

static void handle_rpc_func_cmd_wq(struct optee *optee,
				   struct optee_msg_arg *arg)
{
	if (arg->num_params != 1)
		goto bad;

	if ((arg->params[0].attr & OPTEE_MSG_ATTR_TYPE_MASK) !=
	    OPTEE_MSG_ATTR_TYPE_VALUE_INPUT)
		goto bad;

	switch (arg->params[0].u.value.a) {
	case OPTEE_RPC_WAIT_QUEUE_SLEEP:
		wq_sleep(&optee->wait_queue, arg->params[0].u.value.b);
		break;
	case OPTEE_RPC_WAIT_QUEUE_WAKEUP:
		wq_wakeup(&optee->wait_queue, arg->params[0].u.value.b);
		break;
	default:
		goto bad;
	}

	arg->ret = TEEC_SUCCESS;
	return;
bad:
	arg->ret = TEEC_ERROR_BAD_PARAMETERS;
}

static void handle_rpc_func_cmd_wait(struct optee_msg_arg *arg)
{
	u32 msec_to_wait;

	if (arg->num_params != 1)
		goto bad;

	if ((arg->params[0].attr & OPTEE_MSG_ATTR_TYPE_MASK) !=
	    OPTEE_MSG_ATTR_TYPE_VALUE_INPUT)
		goto bad;

	msec_to_wait = arg->params[0].u.value.a;

	/* Go to interruptible sleep */
	msleep_interruptible(msec_to_wait);

	arg->ret = TEEC_SUCCESS;
	return;
bad:
	arg->ret = TEEC_ERROR_BAD_PARAMETERS;
}

static void handle_rpc_supp_cmd(struct tee_context *ctx, struct optee *optee,
				struct optee_msg_arg *arg)
{
	struct tee_param *params;

	arg->ret_origin = TEEC_ORIGIN_COMMS;

	params = kmalloc_array(arg->num_params, sizeof(struct tee_param),
			       GFP_KERNEL);
	if (!params) {
		arg->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return;
	}

	if (optee->ops->from_msg_param(optee, params, arg->num_params,
				       arg->params)) {
		arg->ret = TEEC_ERROR_BAD_PARAMETERS;
		goto out;
	}

	arg->ret = optee_supp_thrd_req(ctx, arg->cmd, arg->num_params, params);

	if (optee->ops->to_msg_param(optee, arg->params, arg->num_params,
				     params)) {
		arg->ret = TEEC_ERROR_BAD_PARAMETERS;
	}
out:
	kfree(params);
}

struct tee_shm *optee_rpc_cmd_alloc_suppl(struct tee_context *ctx, size_t sz)
{
	u32 ret;
	struct tee_param param;
	struct optee *optee = tee_get_drvdata(ctx->teedev);
	struct tee_shm *shm;

	param.attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT;
	param.u.value.a = OPTEE_RPC_SHM_TYPE_APPL;
	param.u.value.b = sz;
	param.u.value.c = 0;

	ret = optee_supp_thrd_req(ctx, OPTEE_RPC_CMD_SHM_ALLOC, 1, &param);
	if (ret)
		return ERR_PTR(-ENOMEM);

	mutex_lock(&optee->supp.mutex);
	/* Increases count as secure world doesn't have a reference */
	shm = tee_shm_get_from_id(optee->supp.ctx, param.u.value.c);
	mutex_unlock(&optee->supp.mutex);
	return shm;
}

void optee_rpc_cmd_free_suppl(struct tee_context *ctx, struct tee_shm *shm)
{
	struct tee_param param;

	param.attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT;
	param.u.value.a = OPTEE_RPC_SHM_TYPE_APPL;
	param.u.value.b = tee_shm_get_id(shm);
	param.u.value.c = 0;

	/*
	 * Match the tee_shm_get_from_id() in cmd_alloc_suppl() as secure
	 * world has released its reference.
	 *
	 * It's better to do this before sending the request to supplicant
	 * as we'd like to let the process doing the initial allocation to
	 * do release the last reference too in order to avoid stacking
	 * many pending fput() on the client process. This could otherwise
	 * happen if secure world does many allocate and free in a single
	 * invoke.
	 */
	tee_shm_put(shm);

	optee_supp_thrd_req(ctx, OPTEE_RPC_CMD_SHM_FREE, 1, &param);
}

static void handle_rpc_func_cmd_bm_reg(struct optee_msg_arg *arg)
{
	u64 size;
	u64 type;
	u64 paddr;

	if (arg->num_params != 1)
		goto bad;

	if ((arg->params[0].attr & OPTEE_MSG_ATTR_TYPE_MASK) !=
	    OPTEE_MSG_ATTR_TYPE_VALUE_INPUT)
		goto bad;

	type = arg->params[0].u.value.a;
	switch (type) {
	case OPTEE_MSG_RPC_CMD_BENCH_REG_NEW:
		size = arg->params[0].u.value.c;
		paddr = arg->params[0].u.value.b;
		down_write(&optee_bench_ts_rwsem);
		optee_bench_ts_global = memremap(paddr, size, MEMREMAP_WB);
		if (!optee_bench_ts_global) {
			up_write(&optee_bench_ts_rwsem);
			goto bad;
		}
		up_write(&optee_bench_ts_rwsem);
		break;
	case OPTEE_MSG_RPC_CMD_BENCH_REG_DEL:
		down_write(&optee_bench_ts_rwsem);
		if (optee_bench_ts_global)
			memunmap(optee_bench_ts_global);
		optee_bench_ts_global = NULL;
		up_write(&optee_bench_ts_rwsem);
		break;
	default:
		goto bad;
	}

	arg->ret = TEEC_SUCCESS;
	return;
bad:
	arg->ret = TEEC_ERROR_BAD_PARAMETERS;
}

void mitee_rpc_register_callback(uint32_t module_id, uint32_t cmd,
				 uint32_t (*ree_callback_call)(struct optee_msg_param_value *value,
							  void *buf, uint32_t size_in, uint32_t *size_out))
{
	struct optee *optee = get_optee_drv_state();
	struct mitee_rpc_callback_queue *queue = &optee->cb_queue;
	struct callback_item *item = NULL;
	struct callback_item *new_item =  kmalloc(sizeof(*new_item), GFP_KERNEL);
	if (!new_item) {
		pr_err("mitee rpc: failed to alloc memory for callback_item\n");
		return;
	}

	new_item->module_id = module_id;
	new_item->cmd = cmd;
	new_item->callback = ree_callback_call;

	mutex_lock(&queue->mut);
	list_for_each_entry (item, &queue->item, link) {
		// if find the same module_id, replace it
		if (item->module_id == module_id && item->cmd == cmd) {
			list_del(&item->link);
			list_add_tail(&new_item->link, &queue->item);
			mutex_unlock(&queue->mut);
			return;
		}
	}

	list_add_tail(&new_item->link, &queue->item);
	mutex_unlock(&queue->mut);
}
EXPORT_SYMBOL(mitee_rpc_register_callback);

static int optee_msg_arg_to_callback_para(struct optee_msg_arg *arg, struct optee_msg_param_value **value,
					  void **va, uint32_t *size) {
	if (value == NULL || arg == NULL || va == NULL || size == NULL) {
		return -EINVAL;
	}

	if ((arg->params[1].attr & OPTEE_MSG_ATTR_TYPE_MASK) !=
	    OPTEE_MSG_ATTR_TYPE_NONE) {
		*value = &arg->params[1].u.value;
	}

	if ((arg->params[2].attr & OPTEE_MSG_ATTR_TYPE_MASK) !=
	    OPTEE_MSG_ATTR_TYPE_NONE) {
		struct tee_shm *shm = (struct tee_shm *)arg->params[2].u.tmem.shm_ref;
		int rc;
		phys_addr_t pa = 0;
		if (!shm) {
			pr_err("mitee: callback invalid shm\n");
			return -EINVAL;
		}

		rc = tee_shm_get_pa(shm, 0, &pa);
		if (rc) {
			pr_err("mitee: callback invalid shm pa, rc %d\n", rc);
			return rc;
		}

		*va = (char *)tee_shm_get_va(shm, arg->params[2].u.tmem.buf_ptr - pa);
		*size = (uint32_t)arg->params[2].u.tmem.size;
	}

	return 0;
}

static void update_optee_msg_arg(struct optee_msg_arg *arg, uint32_t size) {
	arg->params[2].u.tmem.size = size;
}

static void handle_rpc_cmd_callback(struct optee *optee,
				    struct optee_msg_arg *arg)
{
	struct mitee_rpc_callback_queue *queue = &optee->cb_queue;
	struct callback_item *item = NULL;
	uint32_t module_id = (uint32_t)arg->params[0].u.value.b;
	uint32_t cmd = (uint32_t)arg->params[0].u.value.a;

	mutex_lock(&queue->mut);
	list_for_each_entry (item, &queue->item, link) {
		if (item->module_id == module_id && item->cmd == cmd && item->callback != NULL) {
			if (cmd == OPTEE_REE_CALLBACK_CALL) {
				void *va = NULL;
				uint32_t size_in = 0;
				uint32_t size_out = 0;
				struct optee_msg_param_value *value = NULL;
				int rc;
				rc = optee_msg_arg_to_callback_para(arg, &value, &va, &size_in);
				if (!rc) {
					rc = item->callback(value, va, size_in, &size_out);
					update_optee_msg_arg(arg, size_out);
					arg->ret = rc;
				} else {
					pr_err("mitee callback msg_arg invalid %d\n", rc);
					arg->ret = TEEC_ERROR_BAD_PARAMETERS;
				}
			}
			else {
				arg->ret = item->callback(&arg->params[0].u.value, NULL, 0, NULL);
			}

			mutex_unlock(&queue->mut);
			return;
		}
	}
	mutex_unlock(&queue->mut);
}

void optee_rpc_cmd(struct tee_context *ctx, struct optee *optee,
		   struct optee_msg_arg *arg)
{
	switch (arg->cmd) {
	case OPTEE_RPC_CMD_GET_TIME:
		handle_rpc_func_cmd_get_time(arg);
		break;
	case OPTEE_RPC_CMD_WAIT_QUEUE:
		handle_rpc_func_cmd_wq(optee, arg);
		break;
	case OPTEE_RPC_CMD_SUSPEND:
		handle_rpc_func_cmd_wait(arg);
		break;
	case OPTEE_RPC_CMD_I2C_TRANSFER:
		handle_rpc_func_cmd_i2c_transfer(ctx, arg);
		break;
	case OPTEE_MSG_RPC_CMD_BENCH_REG:
		handle_rpc_func_cmd_bm_reg(arg);
		break;
	case OPTEE_MSG_RPC_CMD_CALLBACK:
		handle_rpc_cmd_callback(optee, arg);
		break;
	default:
		handle_rpc_supp_cmd(ctx, optee, arg);
	}
}
