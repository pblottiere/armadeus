/*
 * MXS PWM driver
 *
 * Copyright (C) 2012 Eric Jarrige <eric.jarrige@armadeus.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h>        /* get_user, copy_to_user */
#include <linux/miscdevice.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/system.h>
#include <mach/device.h>
#include <mach/regs-pwm.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/pm.h>
#include <linux/clk.h>
#include <linux/sysdev.h>
#include <linux/device.h>	/* struct class */
#include <linux/circ_buf.h>
#include <linux/platform_device.h>

#include <linux/slab.h>
#include <linux/sched.h>

#include "mxs-pwm.h"

#define PWM_MIN_FREQUENCY 1		/* Hz */
#define PWM_MAX_FREQUENCY 12000000	/* Hz */


#define DRIVER_NAME         "mxs_pwm"

#define DRIVER_VERSION      "1.00"


struct pwm_device {
	void __iomem *membase;
	unsigned int id;
	int active;
	unsigned int duty;	/* current duty cycle in % x 10 */
	unsigned int frequency;	/* current frequency in Hz */
	unsigned int period_reg;	/* value to put in PWMPR register to have requested freq */

	struct clk *clk;

	struct device *dev;
};

int gMajor = 0;
static void __iomem *pwm_membase = NULL;
static struct pwm_device *dev_table[CONFIG_MXS_PWM_CHANNELS];
static const int div_shift[8] = {0, 1, 2, 3, 4, 6, 8, 10};
static struct class *pwm_class;	/* forward declaration only */

static void inline unregister_sys_file(struct pwm_device *pwm);


static int get_current_pwm_clk_rate(struct pwm_device *pwm)
{
	return clk_get_rate(pwm->clk);
}

/* result in Hz */
static long get_pwm_min_freq(struct pwm_device *pwm)
{
	return PWM_MIN_FREQUENCY;
}

/* result in Hz */
static long get_pwm_max_freq(struct pwm_device *pwm)
{
	return PWM_MAX_FREQUENCY;
}

/* freq in Hz */
static void compute_pwm_params(u32 req_freq, struct pwm_device *pwm)
{
	unsigned int input_freq, cycles, cdiv = 0;

	input_freq = get_current_pwm_clk_rate(pwm);
	pr_debug("input freq, %d %d\n", input_freq, req_freq);

	while (1) {
		cycles = input_freq >> div_shift[cdiv];
		cycles = cycles / req_freq;
		if (cycles < 0x10000)
			break;
		cdiv++;

		if (cdiv > 7)
			return;
	}

	pr_debug("config period_cycles: %d clock divider: %d\n",
			cycles, cdiv);

	pwm->period_reg = BF_PWM_PERIODn_PERIOD(cycles-1) |
		BF_PWM_PERIODn_ACTIVE_STATE(BV_PWM_PERIODn_ACTIVE_STATE__1) |
		BF_PWM_PERIODn_INACTIVE_STATE(BV_PWM_PERIODn_INACTIVE_STATE__0) |
		BF_PWM_PERIODn_CDIV(cdiv);
	pwm->frequency = (input_freq >> div_shift[cdiv]) / cycles;

	pr_debug("period reg: %x freq: %d\n", pwm->period_reg, pwm->frequency);
}

static void setup_pwm_params(struct pwm_device *pwm)
{
	u32 temp = ((pwm->period_reg & BM_PWM_PERIODn_PERIOD)+1)* pwm->duty;

	/* Setup duty cycle */
	writel(((temp<1000)?1:temp/1000) << 16,
		pwm->membase + HW_PWM_ACTIVEn(pwm->id));
	writel(pwm->period_reg, pwm->membase + HW_PWM_PERIODn(pwm->id));
}


/* common setup function whenever something was changed */
static void setup_pwm_unit(struct pwm_device *pwm)
{
	if (pwm->active) {
		/* Activate PWM */
		setup_pwm_params(pwm);
		writel(1 << pwm->id, pwm->membase + HW_PWM_CTRL_SET);
	} else {
		/* De-activate after one period cycle */
		writel(1 << pwm->id, pwm->membase + HW_PWM_CTRL_CLR);
	}
}


static void stop_pwm(struct pwm_device *pwm)
{
	/* De-activate after one period cycle */
	writel(1 << pwm->id, pwm->membase + HW_PWM_CTRL_CLR);
	pwm->active = 0;
	pr_debug("data completed.\n");
	pr_debug("PWMC = 0x%8x\n", readl(pwm->membase + HW_PWM_CTRL));
}

static int init_pwm(struct pwm_device *pwm)
{
	writel((1 << pwm->id) | BM_PWM_CTRL_SFTRST | BM_PWM_CTRL_CLKGATE,
		pwm->membase + HW_PWM_CTRL_CLR);
	pwm->active = 0;
	return 1;
}


/* /DEV INTERFACE */

int pwm_release(struct inode * inode, struct file * filp)
{
	struct pwm_device *pwm = dev_table[iminor(inode)];

	stop_pwm(pwm);

	return 0;
}

int pwm_open(struct inode * inode, struct file * filp)
{
	struct pwm_device *pwm = dev_table[iminor(inode)];

	/* Init PWM hardware */
	init_pwm(pwm);
	
	pr_debug(DRIVER_NAME " opened \n");

	return 0;
}

long pwm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct pwm_device *pwm = dev_table[iminor(filp->f_dentry->d_inode)];

	switch (cmd) {
		/* Set PWM Mode (Tone or Playback) */
		case PWM_IOC_SMODE:
			printk(DRIVER_NAME ": PWM_IOC_SMODE unkown IOCTL\n");
		break;

		/* Set Playback frequency/ouput rate */
		case PWM_IOC_SFREQ:
		{
			compute_pwm_params(arg, pwm);
			setup_pwm_params(pwm);
			pr_debug("Sample rate = %d\n", pwm->frequency);
			pr_debug("ioctl: PWMC = 0x%8x\n", readl(pwm->membase+HW_PWM_CTRL));
		}
		break;

		/* Set samples length (8 or 16 bits) */
		case PWM_IOC_SDATALEN:
			printk(DRIVER_NAME ": PWM_IOC_SDATALEN unkown IOCTL\n");
		break;

		case PWM_IOC_SSAMPLE:
			pwm->duty = arg;
			setup_pwm_params(pwm);
		break;

		case PWM_IOC_SPERIOD:
			printk(DRIVER_NAME ": PWM_IOC_SPERIOD unkown IOCTL\n");
		break;

		case PWM_IOC_STOP:
			stop_pwm(pwm);
		break;

		case PWM_IOC_SWAPDATA:
			printk(DRIVER_NAME ": PWM_IOC_SWAPDATA unkown IOCTL\n");
		break;

		default:
			printk(DRIVER_NAME ": unkown IOCTL\n");
		break;
	}

	return ret;
}


struct file_operations pwm_fops = {
	open:           pwm_open,
	release:        pwm_release,
	unlocked_ioctl:	pwm_ioctl,
};

/* END OF /DEV INTERFACE */


/* /sys interface: */

static ssize_t pwm_show_duty(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret_size = 0;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	ret_size = sprintf(buf, "%u\n", pwm->duty);

	return ret_size;
}

/*
 * Duty value can be 0 ... 1000. Without floating point you can take
 * one position after decimal point into account. The internal counters
 * supports 10 bit resolution, so it makes sense to support such a thing
 * like 66.6% or 33.3% (given as 666 and 333 values)
 */
static ssize_t pwm_store_duty(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	long value;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	value = simple_strtol(buf, NULL, 10);
	if ((value < 1) || (value > 999))
		return -EIO;

	pwm->duty = (unsigned)value;
	setup_pwm_unit(pwm);

	return size;
}

static ssize_t pwm_show_period(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret_size = 0;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	if (pwm->frequency)
		ret_size = sprintf(buf, "%d\n", (u32) 1000000/(pwm->frequency));
	else
		ret_size = sprintf(buf, "0\n");

	return ret_size;
}

static ssize_t pwm_store_period(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	long value;
	struct pwm_device *pwm = dev_get_drvdata(dev);
	
	value = simple_strtol(buf, NULL, 10); /* in us */
	if ((value < (1000000/get_pwm_max_freq(pwm))) || (value > (1000000/get_pwm_min_freq(pwm))))
		return -EIO;

	compute_pwm_params(1000000/value, pwm);
	setup_pwm_unit(pwm);

	return size;
}

static ssize_t pwm_show_frequency(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret_size = 0;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	ret_size = sprintf(buf, "%d\n", pwm->frequency);

	return ret_size;
}

/*
 * Given value (in Hertz) should be between 2 and 100k
 */

static ssize_t pwm_store_frequency(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	long value;

	struct pwm_device *pwm = dev_get_drvdata(dev);

	value = simple_strtol(buf, NULL, 10);
	if ((value < get_pwm_min_freq(pwm)) || (value > get_pwm_max_freq(pwm)))
		return -EIO;

	compute_pwm_params(value, pwm);
	setup_pwm_unit(pwm);

	return size;
}

static ssize_t pwm_show_state(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret_size = 0;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	ret_size = sprintf(buf, "%d\n", pwm->active);

	return ret_size;
}

static ssize_t pwm_store_state(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	long value;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	value = simple_strtol(buf, NULL, 10);
	if (value != 0)
		pwm->active = 1;
	else
		pwm->active = 0;

	setup_pwm_unit(pwm);

	return size;
}

/* dev_attr_duty  /class/pwm/pwmX/duty */
static DEVICE_ATTR(duty, S_IWUSR | S_IRUGO, pwm_show_duty, pwm_store_duty);
/* dev_attr_period  /sys/class/pwm/pwmX/period */
static DEVICE_ATTR(period, S_IWUSR | S_IRUGO, pwm_show_period, pwm_store_period);
/* dev_attr_frequency  /sys/class/pwm/pwmX/frequency */
static DEVICE_ATTR(frequency, S_IWUSR | S_IRUGO, pwm_show_frequency, pwm_store_frequency);
/* dev_attr_active  /sys/class/pwm/pwmX/active */
static DEVICE_ATTR(active, S_IWUSR | S_IRUGO, pwm_show_state, pwm_store_state);



static int imx_pwm_drv_probe(struct platform_device *pdev)
{
	int err = -ENODEV;
	struct resource *res = NULL;
	struct pwm_device *pwm;

	if (pdev->id >= CONFIG_MXS_PWM_CHANNELS)
		dev_err(&pdev->dev, "failed. Unknown module. Remember that this device only supports %d PWM\n", CONFIG_MXS_PWM_CHANNELS);

	if (dev_table[pdev->id] != NULL) {
		dev_err(&pdev->dev, "already in use\n");
		return -ENODEV;
	}


	pwm = kmalloc(sizeof(struct pwm_device), GFP_KERNEL);
	if (!pwm) {
		return -ENOMEM;
	}

	pwm->active = 0;
	pwm->duty = 500; /* = 50.O% */
	pwm->frequency = 1000;
	pwm->id = pdev->id;
	platform_set_drvdata(pdev, pwm);
	dev_table[pdev->id] = pwm;

	if (!pwm_membase) {
		res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		if (!res) {
			dev_err(&pdev->dev, "unable to get mem ressource\n");
			err = -ENOENT;
			goto error_malloc;
		}

		pwm_membase = (void __iomem *)IO_ADDRESS(res->start);
		if (!pwm_membase) {
			err = -ENOMEM;
			goto error_memreg;
		}
	}

	pwm->membase = pwm_membase;

	/* Register our char device */
	if (gMajor == 0) {
		err = register_chrdev(gMajor, DRIVER_NAME, &pwm_fops);
		if (err < 0) {
			dev_err(&pdev->dev, "unable to register char driver\n");
			goto error_iounmap;
		}
	}

	/* Dynamic Major allocation */
	if (gMajor == 0) {
		gMajor = err;
		dev_dbg(&pdev->dev, "major = %d\n", gMajor);
	}

	/* Create /dev */
	pwm->dev = device_create(pwm_class, NULL, MKDEV(gMajor, pdev->id), NULL, "pwm%i", pdev->id);
	if (IS_ERR(pwm->dev)) {
		err = PTR_ERR(pwm->dev);
		dev_err(&pdev->dev, "can't create device\n");
		goto error_platform_init;
	}
	dev_set_drvdata(pwm->dev, pwm);

	pwm->clk = clk_get(&pdev->dev, "pwm");

	if (IS_ERR(pwm->clk)) {
		dev_err(&pdev->dev, "can't get clock\n");
		goto error_platform_init;
	}	
	clk_enable(pwm->clk);

	/* init PWM hardware module */
	init_pwm(pwm);
	compute_pwm_params(pwm->frequency, pwm);
	setup_pwm_unit(pwm);

	/* Register the attributes */
	err = device_create_file(pwm->dev, &dev_attr_duty);
	if (unlikely(err))
		goto error_dev_create;
	err |= device_create_file(pwm->dev, &dev_attr_period);
	if (unlikely(err)) {
		dev_err(&pdev->dev, "can't create /sys/.../period\n");
		goto error_file;
	}
	err |= device_create_file(pwm->dev, &dev_attr_frequency);
	if (unlikely(err)) {
		dev_err(&pdev->dev, "can't create /sys/.../frequency\n");
		goto error_file;
	}
	err |= device_create_file(pwm->dev, &dev_attr_active);
	if (unlikely(err)) {
		dev_err(&pdev->dev, "can't create /sys/.../active\n");
		goto error_file;
	}

	dev_info(&pdev->dev, "initialized\n");
	return 0;

error_file:
	unregister_sys_file(pwm);
error_dev_create:
	device_destroy(pwm_class, MKDEV(gMajor, pdev->id));
error_platform_init:
	unregister_chrdev(gMajor, DRIVER_NAME);
error_iounmap:
        iounmap(pwm->membase);
error_memreg:
	if (res) release_mem_region(res->start, resource_size(res));
error_malloc:
	kfree(pwm);

	return err;
}

static inline void unregister_sys_file(struct pwm_device *pwm)
{
	/* Unregister /sys attributes */
	device_remove_file(pwm->dev, &dev_attr_active);
	device_remove_file(pwm->dev, &dev_attr_period);
	device_remove_file(pwm->dev, &dev_attr_frequency);
	device_remove_file(pwm->dev, &dev_attr_duty);
}

static int imx_pwm_drv_remove(struct platform_device *pdev)
{
	struct resource *res;
	struct pwm_device *pwm = (struct pwm_device*)(dev_get_drvdata(&pdev->dev));

	unregister_sys_file(pwm);
	device_destroy(pwm_class, MKDEV(gMajor, pdev->id));
	unregister_chrdev(gMajor, DRIVER_NAME);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, res->end - res->start + 1);
	kfree(pwm);
	dev_table[pdev->id] = NULL;

	return 0;
}

#ifdef CONFIG_PM
static int imx_pwm_drv_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct pwm_device *pwm = (struct pwm_device*)(dev_get_drvdata(&pdev->dev));

	clk_disable(pwm->clk);
	dev_dbg(&pdev->dev, "suspended\n");

	return 0;
}

static int imx_pwm_drv_resume(struct platform_device *pdev)
{
	struct pwm_device *pwm = (struct pwm_device*)(dev_get_drvdata(&pdev->dev));

	clk_enable(pwm->clk);
	dev_dbg(&pdev->dev, "resumed\n");

	return 0;
}
#else

# define imx_pwm_drv_suspend NULL
# define imx_pwm_drv_resume NULL

#endif /* CONFIG_PM */

static struct platform_driver imx_pwm_driver = {
	.probe      = imx_pwm_drv_probe,
	.remove     = imx_pwm_drv_remove,
	.suspend    = imx_pwm_drv_suspend,
	.resume     = imx_pwm_drv_resume,
	.driver     = {
		.name   = DRIVER_NAME,
	},
};

static int __init imx_pwm_init(void)
{
	printk("i.MX PWM driver v" DRIVER_VERSION "\n");

	pwm_class = class_create(THIS_MODULE, "pwm");
	if (IS_ERR(pwm_class)) {
		return PTR_ERR(pwm_class);
	}

	return platform_driver_register(&imx_pwm_driver);
}

static void __exit imx_pwm_exit(void)
{
	int i;

	platform_driver_unregister(&imx_pwm_driver);
	class_destroy(pwm_class);
	for (i=0; i<CONFIG_MXS_PWM_CHANNELS; i++)
		dev_table[i] = NULL;

	printk(DRIVER_NAME " successfully unloaded\n");
}

module_init(imx_pwm_init);
module_exit(imx_pwm_exit);
MODULE_AUTHOR("Eric Jarrige, inspired by armadeus PWM driver");
MODULE_DESCRIPTION("MXS PWM driver");
MODULE_LICENSE("GPL");

