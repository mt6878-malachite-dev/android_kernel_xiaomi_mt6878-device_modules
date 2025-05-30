// SPDX-License-Identifier: GPL-2.0
//
// Copyright (c) 2021 Mediatek Inc.

#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/property.h>
#include <linux/regmap.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>

#define RT6160_MODE_AUTO	0
#define RT6160_MODE_FPWM	1

#define RT6160_REG_CNTL		0x01
#define RT6160_REG_STATUS	0x02
#define RT6160_REG_DEVID	0x03
#define RT6160_REG_VSELL	0x04
#define RT6160_REG_VSELH	0x05
#define RT6160_NUM_REGS		(RT6160_REG_VSELH + 1)

#define RT6160_FPWM_MASK	BIT(3)
#define RT6160_RAMPRATE_MASK	GENMASK(1, 0)
#define RT6160_VID_MASK		GENMASK(7, 4)
#define RT6160_VSEL_MASK	GENMASK(6, 0)
#define RT6160_HDSTAT_MASK	BIT(4)
#define RT6160_UVSTAT_MASK	BIT(3)
#define RT6160_OCSTAT_MASK	BIT(2)
#define RT6160_TSDSTAT_MASK	BIT(1)
#define RT6160_PGSTAT_MASK	BIT(0)

#define SM5811_RANGE_MASK	BIT(6)
#define SM5811_EN_OUTPUT_MASK	BIT(5)
#define SM5811_FPWM_MASK	BIT(3)
#define SM5811_SLEW_RATE_MASK	GENMASK(1, 0)
#define SM5811_VID_MASK		GENMASK(7, 4)
#define SM5811_VOUT_MASK	GENMASK(7, 0)

#define SM5811_TSD_FLAG_MASK	BIT(1)
#define SM5811_nPOK_FLAG_MASK	BIT(0)

#define RT6160_VENDOR_ID	0xA0
#define RT6160_VOUT_MINUV	2025000
#define RT6160_VOUT_MAXUV	5200000
#define RT6160_VOUT_STPUV	25000
#define RT6160_N_VOUTS		((RT6160_VOUT_MAXUV - RT6160_VOUT_MINUV) / RT6160_VOUT_STPUV + 1)

#define RT6160_I2CRDY_TIMEUS	100
#define SM5811_I2CRDY_TIMEUS	250

#define SM5811_VENDOR_ID	0x01
#define SM5811_VOUT_RL_MINUV	1800000
#define SM5811_VOUT_RL_MAXUV	5275000
#define SM5811_VOUT_RH_MINUV	2025000
#define SM5811_VOUT_RH_MAXUV	5500000
#define SM5811_VOUT_STPUV	25000
#define SM5811_N_RL_VOUTS	((SM5811_VOUT_RL_MAXUV - SM5811_VOUT_RL_MINUV) / SM5811_VOUT_STPUV + 1)
#define SM5811_N_RH_VOUTS	((SM5811_VOUT_RH_MAXUV - SM5811_VOUT_RH_MINUV) / SM5811_VOUT_STPUV + 1)

struct rt6160_priv {
	struct regulator_desc desc;
	struct gpio_desc *enable_gpio;
	struct regmap *regmap;
	bool enable_state;
	bool enable_range;
	bool is_sm5811;
};

static int sm5811_enable_range(struct rt6160_priv *priv)
{
	if (!priv->enable_range)
		return 0;

	return regmap_update_bits(priv->regmap, RT6160_REG_CNTL, SM5811_RANGE_MASK, priv->enable_range);
}

static int rt6160_enable(struct regulator_dev *rdev)
{
	struct rt6160_priv *priv = rdev_get_drvdata(rdev);

	if (!priv->enable_gpio)
		return 0;

	gpiod_set_value_cansleep(priv->enable_gpio, 1);
	priv->enable_state = true;

	if (priv->is_sm5811)
		usleep_range(SM5811_I2CRDY_TIMEUS, SM5811_I2CRDY_TIMEUS + 100);
	else
		usleep_range(RT6160_I2CRDY_TIMEUS, RT6160_I2CRDY_TIMEUS + 100);

	regcache_cache_only(priv->regmap, false);
	return regcache_sync(priv->regmap);
}

static int rt6160_disable(struct regulator_dev *rdev)
{
	struct rt6160_priv *priv = rdev_get_drvdata(rdev);

	if (!priv->enable_gpio)
		return -EINVAL;

	/* Mark regcache as dirty and cache only before HW disabled */
	regcache_cache_only(priv->regmap, true);
	regcache_mark_dirty(priv->regmap);

	priv->enable_state = false;
	gpiod_set_value_cansleep(priv->enable_gpio, 0);

	return 0;
}

static int rt6160_is_enabled(struct regulator_dev *rdev)
{
	struct rt6160_priv *priv = rdev_get_drvdata(rdev);

	return priv->enable_state ? 1 : 0;
}

static int rt6160_set_mode(struct regulator_dev *rdev, unsigned int mode)
{
	struct regmap *regmap = rdev_get_regmap(rdev);
	unsigned int mode_val;

	switch (mode) {
	case REGULATOR_MODE_FAST:
		mode_val = RT6160_FPWM_MASK;
		break;
	case REGULATOR_MODE_NORMAL:
		mode_val = 0;
		break;
	default:
		dev_err(&rdev->dev, "mode not supported\n");
		return -EINVAL;
	}

	return regmap_update_bits(regmap, RT6160_REG_CNTL, RT6160_FPWM_MASK, mode_val);
}

static unsigned int rt6160_get_mode(struct regulator_dev *rdev)
{
	struct regmap *regmap = rdev_get_regmap(rdev);
	unsigned int val;
	int ret;

	ret = regmap_read(regmap, RT6160_REG_CNTL, &val);
	if (ret)
		return ret;

	if (val & RT6160_FPWM_MASK)
		return REGULATOR_MODE_FAST;

	return REGULATOR_MODE_NORMAL;
}

static int rt6160_set_suspend_voltage(struct regulator_dev *rdev, int uV)
{
	struct regmap *regmap = rdev_get_regmap(rdev);
	struct rt6160_priv *priv = rdev_get_drvdata(rdev);
	unsigned int suspend_vsel_reg;
	int vsel;

	vsel = regulator_map_voltage_linear(rdev, uV, uV);
	if (vsel < 0)
		return vsel;

	if (rdev->desc->vsel_reg == RT6160_REG_VSELL)
		suspend_vsel_reg = RT6160_REG_VSELH;
	else
		suspend_vsel_reg = RT6160_REG_VSELL;

	if (priv->is_sm5811)
		return regmap_update_bits(regmap, suspend_vsel_reg,
				  SM5811_VOUT_MASK, vsel);
	else
		return regmap_update_bits(regmap, suspend_vsel_reg,
				  RT6160_VSEL_MASK, vsel);
}

static int rt6160_set_ramp_delay(struct regulator_dev *rdev, int target)
{
	struct regmap *regmap = rdev_get_regmap(rdev);
	const int ramp_tables[] = { 1000, 2500, 5000, 10000 };
	unsigned int i, sel;

	/* Find closest larger or equal */
	for (i = 0; i < ARRAY_SIZE(ramp_tables); i++) {
		sel = i;

		/* If ramp delay is equal to 0, directly set ramp speed to fastest */
		if (target == 0) {
			sel = ARRAY_SIZE(ramp_tables) - 1;
			break;
		}

		if (target <= ramp_tables[i])
			break;
	}

	sel <<= ffs(RT6160_RAMPRATE_MASK) - 1;

	return regmap_update_bits(regmap, RT6160_REG_CNTL, RT6160_RAMPRATE_MASK, sel);
}

static int rt6160_get_error_flags(struct regulator_dev *rdev, unsigned int *flags)
{
	struct regmap *regmap = rdev_get_regmap(rdev);
	struct rt6160_priv *priv = rdev_get_drvdata(rdev);
	unsigned int val, events = 0;
	int ret;

	ret = regmap_read(regmap, RT6160_REG_STATUS, &val);
	if (ret)
		return ret;

	if (priv->is_sm5811) {
		if (val & SM5811_TSD_FLAG_MASK)
			events |= REGULATOR_ERROR_OVER_TEMP;

		if (val & SM5811_nPOK_FLAG_MASK)
			events |= REGULATOR_ERROR_FAIL;
        } else {
		if (val & (RT6160_HDSTAT_MASK | RT6160_TSDSTAT_MASK))
			events |= REGULATOR_ERROR_OVER_TEMP;

		if (val & RT6160_UVSTAT_MASK)
			events |= REGULATOR_ERROR_UNDER_VOLTAGE;

		if (val & RT6160_OCSTAT_MASK)
			events |= REGULATOR_ERROR_OVER_CURRENT;

		if (val & RT6160_PGSTAT_MASK)
			events |= REGULATOR_ERROR_FAIL;
        }

	*flags = events;
	return 0;
}

static const struct regulator_ops rt6160_regulator_ops = {
	.list_voltage = regulator_list_voltage_linear,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,

	.enable = rt6160_enable,
	.disable = rt6160_disable,
	.is_enabled = rt6160_is_enabled,

	.set_mode = rt6160_set_mode,
	.get_mode = rt6160_get_mode,
	.set_suspend_voltage = rt6160_set_suspend_voltage,
	.set_ramp_delay = rt6160_set_ramp_delay,
	.get_error_flags = rt6160_get_error_flags,
};

static unsigned int rt6160_of_map_mode(unsigned int mode)
{
	switch (mode) {
	case RT6160_MODE_FPWM:
		return REGULATOR_MODE_FAST;
	case RT6160_MODE_AUTO:
		return REGULATOR_MODE_NORMAL;
	}

	return REGULATOR_MODE_INVALID;
}

static bool rt6160_is_accessible_reg(struct device *dev, unsigned int reg)
{
	if (reg >= RT6160_REG_CNTL && reg <= RT6160_REG_VSELH)
		return true;
	return false;
}

static bool rt6160_is_volatile_reg(struct device *dev, unsigned int reg)
{
	if (reg == RT6160_REG_STATUS)
		return true;
	return false;
}

static const struct regmap_config rt6160_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = RT6160_REG_VSELH,
	.num_reg_defaults_raw = RT6160_NUM_REGS,
	.cache_type = REGCACHE_FLAT,

	.writeable_reg = rt6160_is_accessible_reg,
	.readable_reg = rt6160_is_accessible_reg,
	.volatile_reg = rt6160_is_volatile_reg,
};

static int rt6160_probe(struct i2c_client *i2c)
{
	struct rt6160_priv *priv;
	struct regulator_config regulator_cfg = {};
	struct regulator_dev *rdev;
	bool vsel_active_low;
	unsigned int devid;
	int ret;

	priv = devm_kzalloc(&i2c->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	vsel_active_low =
		device_property_present(&i2c->dev, "richtek,vsel-active-low");


	priv->enable_gpio = devm_gpiod_get_optional(&i2c->dev, "enable", GPIOD_OUT_HIGH);
	if (IS_ERR(priv->enable_gpio)) {
		dev_err(&i2c->dev, "Failed to get 'enable' gpio\n");
		return PTR_ERR(priv->enable_gpio);
	}
	priv->enable_state = true;

	usleep_range(RT6160_I2CRDY_TIMEUS, RT6160_I2CRDY_TIMEUS + 100);

	priv->regmap = devm_regmap_init_i2c(i2c, &rt6160_regmap_config);
	if (IS_ERR(priv->regmap)) {
		ret = PTR_ERR(priv->regmap);
		dev_err(&i2c->dev, "Failed to init regmap (%d)\n", ret);
		return ret;
	}

	ret = regmap_read(priv->regmap, RT6160_REG_DEVID, &devid);
	if (ret) {
		return ret;
	}

	if (((devid & RT6160_VID_MASK) != RT6160_VENDOR_ID) &&
		((devid & RT6160_VID_MASK) != (SM5811_VENDOR_ID << 4))) {
		dev_err(&i2c->dev, "VID not correct [0x%02x]\n", devid);
		return -ENODEV;
	}

	if ((devid & RT6160_VID_MASK) == (SM5811_VENDOR_ID << 4)) {
		priv->is_sm5811 = true;
	} else {
		priv->is_sm5811 = false;
	}

	if (priv->is_sm5811) {
		priv->enable_range =
			device_property_present(&i2c->dev, "sm,enable-range");
		sm5811_enable_range(priv);
        }

	priv->desc.name = "rt6160-buckboost";
	priv->desc.type = REGULATOR_VOLTAGE;
	priv->desc.owner = THIS_MODULE;
	if (priv->is_sm5811) {
		if(priv->enable_range) {
			priv->desc.min_uV = SM5811_VOUT_RH_MINUV;
			priv->desc.uV_step = SM5811_VOUT_STPUV;
			priv->desc.n_voltages = SM5811_N_RH_VOUTS;
		} else {
			priv->desc.min_uV = SM5811_VOUT_RL_MINUV;
			priv->desc.uV_step = SM5811_VOUT_STPUV;
			priv->desc.n_voltages = SM5811_N_RL_VOUTS;
		}
		priv->desc.vsel_mask = SM5811_VOUT_MASK;
	} else {
		priv->desc.min_uV = RT6160_VOUT_MINUV;
		priv->desc.uV_step = RT6160_VOUT_STPUV;
		priv->desc.n_voltages = RT6160_N_VOUTS;
		priv->desc.vsel_mask = RT6160_VSEL_MASK;
	}
	if (vsel_active_low)
		priv->desc.vsel_reg = RT6160_REG_VSELL;
	else
		priv->desc.vsel_reg = RT6160_REG_VSELH;
	priv->desc.of_map_mode = rt6160_of_map_mode;
	priv->desc.ops = &rt6160_regulator_ops;

	regulator_cfg.dev = &i2c->dev;
	regulator_cfg.of_node = i2c->dev.of_node;
	regulator_cfg.regmap = priv->regmap;
	regulator_cfg.driver_data = priv;
	regulator_cfg.init_data = of_get_regulator_init_data(&i2c->dev, i2c->dev.of_node,
							     &priv->desc);

	rdev = devm_regulator_register(&i2c->dev, &priv->desc, &regulator_cfg);
	if (IS_ERR(rdev)) {
		dev_err(&i2c->dev, "Failed to register regulator\n");
		return PTR_ERR(rdev);
	}

	ret = devm_of_platform_populate(&i2c->dev);
	if (ret) {
		dev_notice(&i2c->dev, "Failed to add platform device\n");
		return ret;
	}

	return 0;
}

static const struct of_device_id __maybe_unused rt6160_of_match_table[] = {
	{ .compatible = "richtek,rt6160", },
	{}
};
MODULE_DEVICE_TABLE(of, rt6160_of_match_table);

static struct i2c_driver rt6160_driver = {
	.driver = {
		.name = "rt6160",
		.of_match_table = rt6160_of_match_table,
	},
	.probe_new = rt6160_probe,
};
module_i2c_driver(rt6160_driver);

MODULE_AUTHOR("ChiYuan Huang <cy_huang@richtek.com>");
MODULE_DESCRIPTION("Richtek RT6160 Voltage Regulator Driver");
MODULE_LICENSE("GPL v2");
