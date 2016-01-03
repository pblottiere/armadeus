/*
 * mxs_lradc.c - driver for the MXS LRADC module
 *
 * Copyright (c) 2012 Eric Jarrige <eric.jarrige@armadeus.org>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/sysfs.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include <mach/hardware.h>
#include <mach/lradc.h>
#include <mach/device.h>
#include <mach/regs-lradc.h>

#include "mxs_lradc.h"

#define MXS_MAX_VALUE 3700 /* mV */

struct mxs_lradc {
	struct device *hwmon_dev;
	struct mutex lock;
	unsigned int base;
};

static ssize_t mxs_lradc_read(struct device *dev,
			   struct device_attribute *devattr, char *buf)
{
	struct mxs_lradc *adc = dev_get_drvdata(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(devattr);
	int status = 0;
	int ret_value;

	if (mutex_lock_interruptible(&adc->lock))
		return -ERESTARTSYS;

	/* Clear the Soft Reset and Clock Gate for normal operation */
	__raw_writel(BM_LRADC_CTRL0_SFTRST | BM_LRADC_CTRL0_CLKGATE,
		     adc->base + HW_LRADC_CTRL0_CLR);

	/* Clear the accumulator & NUM_SAMPLES */
	__raw_writel(0xFFFFFFFF,
		     adc->base + HW_LRADC_CHn_CLR(attr->index));

	/* Clear the interrupt flag */
	__raw_writel(BM_LRADC_CTRL1_LRADC0_IRQ_EN << attr->index,
		     adc->base + HW_LRADC_CTRL1_CLR);

	/*
	 * Get value; this is the max scale value schedule the channel:
	 */
	__raw_writel(BF_LRADC_CTRL0_SCHEDULE(1 << attr->index),
		     adc->base + HW_LRADC_CTRL0_SET);

	/* wait for completion */
	while ((__raw_readl(adc->base + HW_LRADC_CTRL1)
		& (BM_LRADC_CTRL1_LRADC0_IRQ << attr->index)) !=
			(BM_LRADC_CTRL1_LRADC0_IRQ << attr->index))
		cpu_relax();

	/* Clear the interrupt flag */
	__raw_writel((BM_LRADC_CTRL1_LRADC0_IRQ << attr->index),
		     adc->base + HW_LRADC_CTRL1_CLR);

	/* read channel value. */
	ret_value = __raw_readl(adc->base + HW_LRADC_CHn(attr->index));
	ret_value = (ret_value & BM_LRADC_CHn_VALUE) /
			((ret_value & BM_LRADC_CHn_NUM_SAMPLES) + 1);

	/* Adjust scaling factor */ 
	if (!(__raw_readl(adc->base + HW_LRADC_CTRL2)
		& BF_LRADC_CTRL2_DIVIDE_BY_TWO(1 << attr->index)))
		ret_value /= 2;

	status = sprintf(buf, "%d\n", ret_value * MXS_MAX_VALUE / 4096);

	mutex_unlock(&adc->lock);
	return status;
}

static ssize_t mxs_lradc_show_min(struct device *dev,
		struct device_attribute *devattr, char *buf)
{
	return sprintf(buf, "0\n");
}

static ssize_t mxs_lradc_show_max(struct device *dev,
		struct device_attribute *devattr, char *buf)
{
	return sprintf(buf, "%d\n", MXS_MAX_VALUE);
}

static ssize_t mxs_lradc_show_name(struct device *dev, struct device_attribute
				*devattr, char *buf)
{
	return sprintf(buf, "mxs_lradc\n");
}

static struct sensor_device_attribute mxs_lradc_attr[] = {
	SENSOR_ATTR(name, S_IRUGO, mxs_lradc_show_name, NULL, 0),
	SENSOR_ATTR(in_min, S_IRUGO, mxs_lradc_show_min, NULL, 0),
	SENSOR_ATTR(in_max, S_IRUGO, mxs_lradc_show_max, NULL, 0),
};

static struct sensor_device_attribute mxs_lradc_input[] = {
	SENSOR_ATTR(in0_input, S_IRUGO, mxs_lradc_read, NULL, 0),
	SENSOR_ATTR(in1_input, S_IRUGO, mxs_lradc_read, NULL, 1),
	SENSOR_ATTR(in2_input, S_IRUGO, mxs_lradc_read, NULL, 2),
	SENSOR_ATTR(in3_input, S_IRUGO, mxs_lradc_read, NULL, 3),
	SENSOR_ATTR(in4_input, S_IRUGO, mxs_lradc_read, NULL, 4),
	SENSOR_ATTR(in5_input, S_IRUGO, mxs_lradc_read, NULL, 5),
	SENSOR_ATTR(in6_input, S_IRUGO, mxs_lradc_read, NULL, 6),
	SENSOR_ATTR(in7_input, S_IRUGO, mxs_lradc_read, NULL, 7),
};
/*----------------------------------------------------------------------*/

static int __devinit mxs_lradc_probe(struct platform_device *pdev)
{
	struct mxs_lradc *adc;
	struct resource *res;
	struct mxs_lradc_platform_data *pdata = pdev->dev.platform_data;
	int status;
	int i;

	adc = kzalloc(sizeof(struct mxs_lradc), GFP_KERNEL);
	if (adc == NULL)
		return -ENOMEM;

	mutex_init(&adc->lock);
	mutex_lock(&adc->lock);

	platform_set_drvdata(pdev, adc);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "%s: couldn't get MEM resource\n", __func__);
		status = -ENODEV;
		goto exit_free;
	}
	adc->base = (unsigned int)IO_ADDRESS(res->start);

	if (pdata) {
		for (i = 0; i < pdata->num_lradc; i++) {
			hw_lradc_use_channel(pdata->lradc[i].channel);
			hw_lradc_configure_channel(pdata->lradc[i].channel,
				pdata->lradc[i].enable_div2,
				pdata->lradc[i].samples,
				pdata->lradc[i].samples);
			__raw_writel(0x0F << (4 * pdata->lradc[i].channel),
				adc->base + HW_LRADC_CTRL4_CLR);
			__raw_writel( pdata->lradc[i].mux_pad <<
				(4 * pdata->lradc[i].channel),
				adc->base + HW_LRADC_CTRL4_SET);
			status = device_create_file(&pdev->dev,
			&mxs_lradc_input[pdata->lradc[i].channel].dev_attr);
			if (status) {
				dev_err(&pdev->dev, "device_create_file failed.\n");
				goto exit_free;
			}
		}
	}

	/* Register sysfs hooks */
	for (i = 0; i < ARRAY_SIZE(mxs_lradc_attr); i++) {
		status = device_create_file(&pdev->dev,
						&mxs_lradc_attr[i].dev_attr);
		if (status) {
			dev_err(&pdev->dev, "device_create_file failed.\n");
			goto exit_free;
		}
	}

	adc->hwmon_dev = hwmon_device_register(&pdev->dev);
	if (IS_ERR(adc->hwmon_dev)) {
		dev_err(&pdev->dev, "hwmon_device_register failed.\n");
		status = PTR_ERR(adc->hwmon_dev);
		goto exit_remove;
	}

	mutex_unlock(&adc->lock);
	return 0;

exit_remove:
	for (i--; i >= 0; i--)
		device_remove_file(&pdev->dev, &mxs_lradc_attr[i].dev_attr);
exit_free:
	dev_set_drvdata(&pdev->dev, NULL);
	mutex_unlock(&adc->lock);
	kfree(adc);
	return status;
}

static int __devexit mxs_lradc_remove(struct platform_device *pdev)
{
	struct mxs_lradc *adc = dev_get_drvdata(&pdev->dev);
	struct mxs_lradc_platform_data *pdata = pdev->dev.platform_data;
	int i;

	mutex_lock(&adc->lock);
	hwmon_device_unregister(adc->hwmon_dev);
	for (i = 0; i < ARRAY_SIZE(mxs_lradc_attr); i++)
		device_remove_file(&pdev->dev, &mxs_lradc_attr[i].dev_attr);

	if (pdata) {
		for (i = 0; i < pdata->num_lradc; i++) {
			hw_lradc_unuse_channel(pdata->lradc[i].channel);
			device_remove_file(&pdev->dev,
			&mxs_lradc_input[pdata->lradc[i].channel].dev_attr);
		}
	}

	dev_set_drvdata(&pdev->dev, NULL);
	mutex_unlock(&adc->lock);
	kfree(adc);

	return 0;
}

static struct platform_driver mxs_lradc_driver = {
	.probe      = mxs_lradc_probe,
	.remove     = mxs_lradc_remove,
	.driver     = {
		.name   = "mxs_lradc",
		.owner	= THIS_MODULE,
	},
};

/* Init module */

static int __init init_mxs_lradc(void)
{
	return platform_driver_register(&mxs_lradc_driver);
}

static void __exit exit_mxs_lradc(void)
{
	platform_driver_unregister(&mxs_lradc_driver);
}

module_init(init_mxs_lradc);
module_exit(exit_mxs_lradc);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eric Jarrige <eric.jarrige@armadeus.org>");
MODULE_DESCRIPTION("MXC LRADC driver");
