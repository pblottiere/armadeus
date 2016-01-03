/*
 * Linux kernel driver for the POD component pod_pwm
 *
 * (C) Copyright 2012 - EIRBOT <eirbot.enseirb.fr>
 * Author: Kevin JOLY joly.kevin25@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
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
#include <linux/module.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include "pod_pwm.h"

/* POD_PWM Registers*/
#define REG_PWM_ID 0x00
#define REG_T_ON 0x02
#define REG_DIV 0x04

#define DEFAULT_CLOCK_SOURCE_FREQUENCY 100000000

struct class *class_pwm;

struct pod_pwm_device {
	int angle;
	void __iomem *membase;
	struct device *dev;
	int gMajor;
	unsigned long clock_source;
};

const struct file_operations pwm_fops = {
	/* TODO */
};

/* Low level access */
static u16 read_reg(const struct pod_pwm_device *pwm, u16 reg)
{
	return ioread16(pwm->membase + reg);
}

void write_reg(const struct pod_pwm_device *pwm, u16 reg, u16 value)
{
	iowrite16(value, pwm->membase + reg);
}

unsigned int get_duty(const struct pod_pwm_device *pwm)
{
	u16 t_on;
	u16 div;

	t_on = read_reg(pwm, REG_T_ON);
	div = read_reg(pwm, REG_DIV);

	if (div < 1)
		return 0;

	return (t_on*1000)/div;
}

/* Attributes */

static ssize_t pwm_show_duty(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct pod_pwm_device *pwm =
		(struct pod_pwm_device *)(dev_get_drvdata(dev));

	return sprintf(buf, "%d\n", get_duty(pwm));
}

static ssize_t pwm_store_duty(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	int err;
	u16 div;
	u16 duty;
	struct pod_pwm_device *pwm =
		(struct pod_pwm_device *)(dev_get_drvdata(dev));

	err = kstrtouint(buf, 10, (unsigned int *)(&duty));

	if (err) {
		dev_err(dev, "kstrtol failed with error code %d.\n", err);
		return size;
	}

	if (duty < 0)
		duty = 0;
	if (duty > 1000)
		duty = 1000;

	div = read_reg(pwm, REG_DIV);
	write_reg(pwm, REG_T_ON, (div * duty)/1000);

	return size;
}

static ssize_t pwm_show_frequency(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	u16 div;
	struct pod_pwm_device *pwm =
		(struct pod_pwm_device *)(dev_get_drvdata(dev));

	div = read_reg(pwm, REG_DIV);
	if (div <= 0)
		return pwm->clock_source;

	return sprintf(buf, "%ld\n", pwm->clock_source/div);
}

static ssize_t pwm_store_frequency(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	u16 div;
	int err;
	unsigned int duty;
	unsigned long freq;
	struct pod_pwm_device *pwm =
		(struct pod_pwm_device *)(dev_get_drvdata(dev));

	err = kstrtol(buf, 10, &freq);

	if (err) {
		dev_err(dev, "kstrtol failed with error code %d.\n", err);
		return size;
	}

	if ((freq > pwm->clock_source) || (freq < pwm->clock_source/65635)) {
		dev_err(dev, "Invalid pwm frequency %ld.\n", freq);
		return size;
	}

	duty = get_duty(pwm);
	div = pwm->clock_source / freq;

	write_reg(pwm, REG_DIV, div);
	write_reg(pwm, REG_T_ON, (div * duty)/1000);

	return size;
}

static ssize_t pwm_show_clock_source(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct pod_pwm_device *pwm =
		(struct pod_pwm_device *)(dev_get_drvdata(dev));

	return sprintf(buf, "%ld\n", pwm->clock_source);
}

static ssize_t pwm_store_clock_source(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int err;
	long freq;
	struct pod_pwm_device *pwm =
		(struct pod_pwm_device *)(dev_get_drvdata(dev));

	err = kstrtol(buf, 10, &freq);

	if (err) {
		dev_err(dev, "kstrtol failed with error code %d.\n", err);
		return size;
	}

	if (freq < 1)
		return 0;

	pwm->clock_source = freq;

	return size;
}

/* dev_attr_duty  /class/pwm/pwmX/duty */
static DEVICE_ATTR(duty, S_IWUSR | S_IRUGO, pwm_show_duty, pwm_store_duty);
/* dev_attr_frequency  /sys/class/pwm/pwmX/frequency */
static DEVICE_ATTR(frequency, S_IWUSR | S_IRUGO, pwm_show_frequency,
	pwm_store_frequency);
/* dev_attr_clock_source  /sys/class/pwm/pwmX/clock_source */
static DEVICE_ATTR(clock_source, S_IWUSR | S_IRUGO, pwm_show_clock_source,
	pwm_store_clock_source);

void init_pwm(struct pod_pwm_device *pwm)
{
	pwm->clock_source = DEFAULT_CLOCK_SOURCE_FREQUENCY;
	pwm->gMajor = 0;
}

static int pod_pwm_drv_probe(struct platform_device *pdev)
{
	struct resource *resource_memory;
	struct pod_pwm_device *pwm;
	int err;
	u16 id;

	resource_memory = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (!resource_memory) {
		err = -ENODEV;
		dev_err(&pdev->dev, "Device pod_pwm not found\n");
		goto exit_error;
	}

	if (!request_mem_region(resource_memory->start,
		resource_size(resource_memory), DRIVER_NAME)) {
		dev_err(&pdev->dev, "Can't request memory region %x to %x\n",
			resource_memory->start,
			resource_memory->start + resource_memory->end);
		err = -ENOMEM;
		goto exit_error;
	}

	pwm = kmalloc(sizeof(struct pod_pwm_device), GFP_KERNEL);

	if (!pwm) {
		err = -ENOMEM;
		goto exit_release_mem;
	}
	pwm->membase = ioremap(resource_memory->start,
			resource_memory->end - resource_memory->start + 1);
	if (!pwm) {
		err = -ENOMEM;
		goto exit_kfree;
	}

	id = read_reg(pwm, REG_PWM_ID);

	if (pdev->id != id) {
		dev_err(&pdev->dev,
			"Driver id %d doesn't match with the device id %d\n",
			pdev->id, id);
		err = -ENODEV;
		goto exit_iounmap;
	}

	init_pwm(pwm);

	err = register_chrdev(pwm->gMajor, DRIVER_NAME, &pwm_fops);
	if (err < 0) {
		dev_err(&pdev->dev, "unable to register char driver.\n");
		goto exit_iounmap;
	}

	if (!pwm->gMajor)
		pwm->gMajor = err;

	pwm->dev = device_create(class_pwm, NULL, MKDEV(pwm->gMajor, pdev->id),
			NULL, "pwm%i", pdev->id);

	if (IS_ERR(pwm->dev)) {
		err = PTR_ERR(pwm->dev);
		dev_err(&pdev->dev, "can't create device\n");
		goto error_device_destroy;
	}

	err = device_create_file(pwm->dev, &dev_attr_duty);
	if (err) {
		dev_err(&pdev->dev,
			"can't create device attribute  \"duty\".\n");
		goto error_device_destroy;
	}

	err = device_create_file(pwm->dev, &dev_attr_frequency);
	if (err) {
		dev_err(&pdev->dev,
			"can't create device attribute \"frequency\".\n");
		goto error_device_destroy;
	}

	err = device_create_file(pwm->dev, &dev_attr_clock_source);

	if (err) {
		dev_err(&pdev->dev,
			"can't create device attribute \"clock_source\".\n");
		goto error_device_destroy;
	}

	dev_set_drvdata(&pdev->dev, pwm);
	dev_set_drvdata(pwm->dev, pwm);

	dev_info(&pdev->dev, "pod_pwm:%d Succesfully probed", id);

	return 0;

error_device_destroy:
	device_destroy(class_pwm, MKDEV(pwm->gMajor, pdev->id));
	unregister_chrdev(pwm->gMajor, DRIVER_NAME);
exit_iounmap:
	iounmap(pwm->membase);
exit_kfree:
	kfree(pwm);
exit_release_mem:
	release_mem_region(resource_memory->start,
		resource_size(resource_memory));
exit_error:
	return err;
}

static int pod_pwm_drv_remove(struct platform_device *pdev)
{
	struct pod_pwm_device *pwm =
		(struct pod_pwm_device *)(dev_get_drvdata(&pdev->dev));
	struct resource *resource_memory =
		platform_get_resource(pdev, IORESOURCE_MEM, 0);

	device_remove_file(pwm->dev, &dev_attr_duty);
	device_remove_file(pwm->dev, &dev_attr_frequency);
	device_remove_file(pwm->dev, &dev_attr_clock_source);

	device_destroy(class_pwm, MKDEV(pwm->gMajor, pdev->id));
	unregister_chrdev(pwm->gMajor, DRIVER_NAME);

	if (pwm->membase > 0)
		iounmap(pwm->membase);

	kfree(pwm);
	release_mem_region(resource_memory->start,
		resource_size(resource_memory));

		return 0;
}

static struct platform_driver pod_pwm_driver = {
	.probe = pod_pwm_drv_probe,
	.remove = pod_pwm_drv_remove,
	.driver = {
		.name = DRIVER_NAME,
},
};

static int __init pod_pwm_init(void)
{
	class_pwm = class_create(THIS_MODULE, "pod_pwm");
	if (IS_ERR(class_pwm))
		return PTR_ERR(class_pwm);

	return platform_driver_register(&pod_pwm_driver);
}

static void __exit pod_pwm_exit(void)
{
	platform_driver_unregister(&pod_pwm_driver);
	class_destroy(class_pwm);
}

module_init(pod_pwm_init);
module_exit(pod_pwm_exit);

MODULE_AUTHOR("Kevin JOLY, inspired by i.MX PWM driver");
MODULE_DESCRIPTION("POD pwm driver");
MODULE_LICENSE("GPL");
