
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
//#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/hardware_info.h>
#include <linux/of_platform.h>
char Lcm_name[HARDWARE_MAX_ITEM_LONGTH];
char Tp_name[HARDWARE_MAX_ITEM_LONGTH];
char Sar_name[HARDWARE_MAX_ITEM_LONGTH];
char board_id[HARDWARE_MAX_ITEM_LONGTH];
static char hardwareinfo_name[HARDWARE_MAX_ITEM][HARDWARE_MAX_ITEM_LONGTH];
char hardware_id[HARDWARE_MAX_ITEM_LONGTH];
char* hardwareinfo_items[HARDWARE_MAX_ITEM] =
{
        "LCD",
        "TP",
        "MEMORY",
        "CAM_FRONT",
        "CAM_BACK",
        "BT",
        "WIFI",
        "GSENSOR",
        "PLSENSOR",
        "GYROSENSOR",
        "MSENSOR",
        "SAR",
        "GPS",
        "FM",
        "BATTERY",
        "CAM_M_BACK",
        "CAM_M_FRONT",
        "BOARD_ID"
};
int hardwareinfo_set_prop(int cmd, const char *name)
{
        if(cmd < 0 || cmd >= HARDWARE_MAX_ITEM)
                return -1;
        strcpy(hardwareinfo_name[cmd], name);
        return 0;
}
int __weak tid_hardware_info_get(char *buf, int size)
{
        snprintf(buf, size, "touch info interface is not ready\n");
        return 0;
}
EXPORT_SYMBOL_GPL(hardwareinfo_set_prop);
static long hardwareinfo_ioctl(struct file *file, unsigned int cmd,unsigned long arg)
{
        int ret = 0, hardwareinfo_num;
        void __user *data = (void __user *)arg;
        //char info[HARDWARE_MAX_ITEM_LONGTH];
        switch (cmd) {
        case HARDWARE_LCD_GET:
                //hardwareinfo_set_prop(HARDWARE_LCD, Lcm_name);
                hardwareinfo_num = HARDWARE_LCD;
                break;
        case HARDWARE_TP_GET:
                hardwareinfo_num = HARDWARE_TP;
                //tid_hardware_info_get(hardwareinfo_name[hardwareinfo_num],
                //                                ARRAY_SIZE(hardwareinfo_name[0]));
                break;
        case HARDWARE_FLASH_GET:
                hardwareinfo_num = HARDWARE_FLASH;
                break;
        case HARDWARE_FRONT_CAM_GET:
                hardwareinfo_num = HARDWARE_FRONT_CAM;
                break;
        case HARDWARE_BACK_CAM_GET:
                hardwareinfo_num = HARDWARE_BACK_CAM;
                break;
        case HARDWARE_BACK_SUBCAM_GET:
                hardwareinfo_num = HARDWARE_BACK_SUBCAM;
                break;
        case HARDWARE_BACK_WIDE_CAM_GET:
                hardwareinfo_num = HARDWARE_BACK_WIDE_CAM;
                break;
        case HARDWARE_BACK_MACRO_CAM_GET:
                hardwareinfo_num = HARDWARE_BACK_MACRO_CAM;
                break;
        case HARDWARE_BT_GET:
                hardwareinfo_set_prop(HARDWARE_BT, "MT6631");
                hardwareinfo_num = HARDWARE_BT;
                break;
        case HARDWARE_WIFI_GET:
                hardwareinfo_set_prop(HARDWARE_WIFI, "MT6631");
                hardwareinfo_num = HARDWARE_WIFI;
                break;
        case HARDWARE_ACCELEROMETER_GET:
                hardwareinfo_num = HARDWARE_ACCELEROMETER;
                break;
        case HARDWARE_ALSPS_GET:
                hardwareinfo_num = HARDWARE_ALSPS;
                break;
        case HARDWARE_GYROSCOPE_GET:
                hardwareinfo_num = HARDWARE_GYROSCOPE;
                break;
        case HARDWARE_MAGNETOMETER_GET:
                hardwareinfo_num = HARDWARE_MAGNETOMETER;
                break;
        case HARDWARE_SAR_GET:
                hardwareinfo_set_prop(HARDWARE_SAR, Sar_name);
                hardwareinfo_num = HARDWARE_SAR;
                break;
        case HARDWARE_GPS_GET:
                hardwareinfo_set_prop(HARDWARE_GPS, "MT6631");
            hardwareinfo_num = HARDWARE_GPS;
                break;
        case HARDWARE_NFC_GET:
              //  hardwareinfo_set_prop(HARDWARE_NFC, "nxp");
                hardwareinfo_num = HARDWARE_NFC;
                break;
        case HARDWARE_FM_GET:
                hardwareinfo_set_prop(HARDWARE_FM, "MT6631");
            hardwareinfo_num = HARDWARE_FM;
                break;
        case HARDWARE_BATTERY_ID_GET:
                hardwareinfo_num = HARDWARE_BATTERY_ID;
                break;
	case HARDWARE_BMS_GAUGE_GET:
		hardwareinfo_num = HARDWARE_BMS_GAUGE;
                break;
	case HARDWARE_CHARGER_IC_INFO_GET:
		hardwareinfo_num = HARDWARE_CHARGER_IC;
		break;
        case HARDWARE_SUB_CHARGER_MASTER_GET:
                hardwareinfo_num = HARDWARE_SUB_CHARGER_MASTER;
                break;
        case HARDWARE_SUB_CHARGER_SLAVE_GET:
                hardwareinfo_num = HARDWARE_SUB_CHARGER_SLAVE;
                break;
        case HARDWARE_USB_PD_GET:
                hardwareinfo_num = HARDWARE_USB_PD;
                break;
        case HARDWARE_VIBRATOR_IC_GET:
                hardwareinfo_num = HARDWARE_VIBRATOR_IC;
                break;
        case HARDWARE_BACK_CAM_MOUDULE_ID_GET:
                hardwareinfo_num = HARDWARE_BACK_CAM_MOUDULE_ID;
                break;
        case HARDWARE_FRONT_CAM_MODULE_ID_GET:
                hardwareinfo_num = HARDWARE_FRONT_CAM_MOUDULE_ID;
                break;
        case HARDWARE_BACK_WIDE_CAM_MOUDULE_ID_GET:
                hardwareinfo_num = HARDWARE_BACK_WIDE_CAM_MOUDULE_ID;
                break;
        case HARDWARE_BACK_MACRO_CAM_MOUDULE_ID_GET:
                hardwareinfo_num = HARDWARE_BACK_MACRO_CAM_MOUDULE_ID;
                break;
        case HARDWARE_BACK_SUBCAM_MODULEID_GET:
                hardwareinfo_num = HARDWARE_BACK_SUBCAM_MODULEID;
                break;
        case HARDWARE_BOARD_ID_GET:
                hardwareinfo_set_prop(HARDWARE_BOARD_ID, board_id);
                hardwareinfo_num = HARDWARE_BOARD_ID;
                break;
        case HARDWARE_HARDWARE_ID_GET:
                hardwareinfo_set_prop(HARDWARE_HARDWARE_ID, hardware_id);
                hardwareinfo_num = HARDWARE_HARDWARE_ID;
		break;
        case HARDWARE_BACK_CAM_MOUDULE_ID_SET:
                if(copy_from_user(hardwareinfo_name[HARDWARE_BACK_CAM_MOUDULE_ID], data,sizeof(data)))
                {
                        pr_err("wgz copy_from_user error");
                        ret =  -EINVAL;
                }
                goto set_ok;
                break;
        case HARDWARE_FRONT_CAM_MODULE_ID_SET:
                if(copy_from_user(hardwareinfo_name[HARDWARE_FRONT_CAM_MOUDULE_ID], data,sizeof(data)))
                {
                        pr_err("wgz copy_from_user error");
                        ret =  -EINVAL;
                }
                goto set_ok;
                break;
        case HARDWARE_SMARTPA_IC_GET:
                pr_info("%s smartpa get ic",__func__);
                hardwareinfo_num = HARDWARE_SMARTPA;
                break;
        default:
                ret = -EINVAL;
                goto err_out;
        }
        //memset(data, 0, HARDWARE_MAX_ITEM_LONGTH);//clear the buffer
        if (copy_to_user(data, hardwareinfo_name[hardwareinfo_num], strlen(hardwareinfo_name[hardwareinfo_num]))){
                //printk("%s, copy to usr error\n", __func__);
                ret =  -EINVAL;
        }
set_ok:
err_out:
        return ret;
}
static ssize_t show_boardinfo(struct device *dev, struct device_attribute *attr, char *buf)
{
        int i = 0;
        char temp_buffer[HARDWARE_MAX_ITEM_LONGTH];
        int buf_size = 0;
        for(i = 0; i < HARDWARE_MAX_ITEM; i++)
        {
                memset(temp_buffer, 0, HARDWARE_MAX_ITEM_LONGTH);
                if(i == HARDWARE_LCD)
                {
                        sprintf(temp_buffer, "%s : %s\n", hardwareinfo_items[i], Lcm_name);
                }
                else if(i == HARDWARE_BT || i == HARDWARE_WIFI || i == HARDWARE_GPS || i == HARDWARE_FM)
                {
                        sprintf(temp_buffer, "%s : %s\n", hardwareinfo_items[i], "Qualcomm");
                }
                else
                {
                        sprintf(temp_buffer, "%s : %s\n", hardwareinfo_items[i], hardwareinfo_name[i]);
                }
                strcat(buf, temp_buffer);
                buf_size +=strlen(temp_buffer);
        }
        return buf_size;
}
static DEVICE_ATTR(boardinfo, S_IRUGO, show_boardinfo, NULL);
static int boardinfo_probe(struct platform_device *pdev)
{
        struct device *dev = &pdev->dev;
        int rc = 0;
        printk("%s: start\n", __func__);
        rc = device_create_file(dev, &dev_attr_boardinfo);
        if (rc < 0)
                return rc;
        dev_info(dev, "%s: ok\n", __func__);
        return 0;
}
static int boardinfo_remove(struct platform_device *pdev)
{
        struct device *dev = &pdev->dev;
        device_remove_file(dev, &dev_attr_boardinfo);
        dev_info(&pdev->dev, "%s\n", __func__);
        return 0;
}
static struct file_operations hardwareinfo_fops = {
        .owner = THIS_MODULE,
        .open = simple_open,
        .unlocked_ioctl = hardwareinfo_ioctl,
        .compat_ioctl = hardwareinfo_ioctl,
};
static struct miscdevice hardwareinfo_device = {
        .minor = MISC_DYNAMIC_MINOR,
        .name = "hardwareinfo",
        .fops = &hardwareinfo_fops,
};
static struct of_device_id boardinfo_of_match[] = {
        { .compatible = "wt:boardinfo", },
        {}
};
static struct platform_driver boardinfo_driver = {
        .driver = {
                .name   = "boardinfo",
                .owner  = THIS_MODULE,
                .of_match_table = boardinfo_of_match,
        },
        .probe          = boardinfo_probe,
        .remove         = boardinfo_remove,
};
static int __init hardwareinfo_init_module(void)
{
        int ret, i;
        for(i = 0; i < HARDWARE_MAX_ITEM; i++)
                strcpy(hardwareinfo_name[i], "NULL");
        ret = misc_register(&hardwareinfo_device);
        if(ret < 0){
                return -ENODEV;
        }
        ret = platform_driver_register(&boardinfo_driver);
        if(ret != 0)
        {
                return -ENODEV;
        }
        return 0;
}
static void __exit hardwareinfo_exit_module(void)
{
        misc_deregister(&hardwareinfo_device);
}
module_init(hardwareinfo_init_module);
module_exit(hardwareinfo_exit_module);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ming He <heming@wingtech.com>");

