/*
 * Linux kernel driver for the POD component pod_servo
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

#include "pod_servo.h"

/* POD_SERVO Registers*/
#define REG_SERVO_ID 0x00
#define REG_ANGLE 0x02

#define DEFAULT_CLOCK_SOURCE_FREQUENCY 100000000

struct class *class_servo;

struct pod_servo_device {
	int angle;
	void __iomem *membase;
	struct device *dev;
	int gMajor;
	unsigned long clock_source;
};

const struct file_operations servo_fops = {
	/* TODO */
};

/* Low level access */
static u16 read_reg(const struct pod_servo_device *servo, u16 reg)
{
	return ioread16(servo->membase + reg);
}

void write_reg(const struct pod_servo_device *servo, u16 reg, u16 value)
{
	iowrite16(value, servo->membase + reg);
}

unsigned int get_angle(const struct pod_servo_device *servo)
{
	u16 angle;

	angle = read_reg(servo, REG_ANGLE);

	return (angle*180)/1000;
}

/* Attributes */

static ssize_t servo_show_angle(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct pod_servo_device *servo =
		(struct pod_servo_device *)(dev_get_drvdata(dev));

	return sprintf(buf, "%d\n", get_angle(servo));
}

static ssize_t servo_store_angle(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	int err;
	u16 angle;
	struct pod_servo_device *servo =
		(struct pod_servo_device *)(dev_get_drvdata(dev));

	err = kstrtouint(buf, 10, (unsigned int *)(&angle));

	if (err) {
		dev_err(dev, "kstrtol failed with error code %d.\n", err);
		return size;
	}

	if (angle < 0)
		angle = 0;
	if (angle > 180)
		angle = 1000;

	write_reg(servo, REG_ANGLE, (angle*1000)/180);

	return size;
}

static ssize_t servo_show_clock_source(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct pod_servo_device *servo =
		(struct pod_servo_device *)(dev_get_drvdata(dev));

	return sprintf(buf, "%ld\n", servo->clock_source);
}

static ssize_t servo_store_clock_source(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int err;
	long freq;
	struct pod_servo_device *servo =
		(struct pod_servo_device *)(dev_get_drvdata(dev));

	err = kstrtol(buf, 10, &freq);

	if (err) {
		dev_err(dev, "kstrtol failed with error code %d.\n", err);
		return size;
	}

	if (freq < 1)
		return 0;

	servo->clock_source = freq;

	return size;
}

/* dev_attr_angle  /class/servo/servoX/angle */
static DEVICE_ATTR(angle, S_IWUSR | S_IRUGO, servo_show_angle,
	servo_store_angle);
/* dev_attr_clock_source  /sys/class/servo/servoX/clock_source */
static DEVICE_ATTR(clock_source, S_IWUSR | S_IRUGO, servo_show_clock_source,
	servo_store_clock_source);

void init_servo(struct pod_servo_device *servo)
{
	servo->clock_source = DEFAULT_CLOCK_SOURCE_FREQUENCY;
	servo->gMajor = 0;
}

static int pod_servo_drv_probe(struct platform_device *pdev)
{
	struct resource *resource_memory;
	struct pod_servo_device *servo;
	int err;
	u16 id;

	resource_memory = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (!resource_memory) {
		err = -ENODEV;
		dev_err(&pdev->dev, "Device pod_servo not found\n");
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

	servo = kmalloc(sizeof(struct pod_servo_device), GFP_KERNEL);

	if (!servo) {
		err = -ENOMEM;
		goto exit_release_mem;
	}
	servo->membase = ioremap(resource_memory->start,
			resource_memory->end - resource_memory->start + 1);
	if (!servo) {
		err = -ENOMEM;
		goto exit_kfree;
	}

	id = read_reg(servo, REG_SERVO_ID);

	if (pdev->id != id) {
		dev_err(&pdev->dev,
			"Driver id %d doesn't match with the device id %d\n",
			pdev->id, id);
		err = -ENODEV;
		goto exit_iounmap;
	}

	init_servo(servo);

	err = register_chrdev(servo->gMajor, DRIVER_NAME, &servo_fops);
	if (err < 0) {
		dev_err(&pdev->dev, "unable to register char driver.\n");
		goto exit_iounmap;
	}

	if (!servo->gMajor)
		servo->gMajor = err;

	servo->dev = device_create(class_servo, NULL, MKDEV(servo->gMajor, pdev->id),
			NULL, "servo%i", pdev->id);

	if (IS_ERR(servo->dev)) {
		err = PTR_ERR(servo->dev);
		dev_err(&pdev->dev, "can't create device\n");
		goto error_device_destroy;
	}

	err = device_create_file(servo->dev, &dev_attr_angle);
	if (err) {
		dev_err(&pdev->dev,
			"can't create device attribute  \"angle\".\n");
		goto error_device_destroy;
	}

	err = device_create_file(servo->dev, &dev_attr_clock_source);

	if (err) {
		dev_err(&pdev->dev,
			"can't create device attribute \"clock_source\".\n");
		goto error_device_destroy;
	}

	dev_set_drvdata(&pdev->dev, servo);
	dev_set_drvdata(servo->dev, servo);

	dev_info(&pdev->dev, "pod_servo:%d Succesfully probed", id);

	return 0;

error_device_destroy:
	device_destroy(class_servo, MKDEV(servo->gMajor, pdev->id));
	unregister_chrdev(servo->gMajor, DRIVER_NAME);
exit_iounmap:
	iounmap(servo->membase);
exit_kfree:
	kfree(servo);
exit_release_mem:
	release_mem_region(resource_memory->start,
		resource_size(resource_memory));
exit_error:
	return err;
}

static int pod_servo_drv_remove(struct platform_device *pdev)
{
	struct pod_servo_device *servo =
		(struct pod_servo_device *)(dev_get_drvdata(&pdev->dev));
	struct resource *resource_memory =
		platform_get_resource(pdev, IORESOURCE_MEM, 0);

	device_remove_file(servo->dev, &dev_attr_angle);
	device_remove_file(servo->dev, &dev_attr_clock_source);

	device_destroy(class_servo, MKDEV(servo->gMajor, pdev->id));
	unregister_chrdev(servo->gMajor, DRIVER_NAME);

	if (servo->membase > 0)
		iounmap(servo->membase);

	kfree(servo);
	release_mem_region(resource_memory->start,
		resource_size(resource_memory));

		return 0;
}

static struct platform_driver pod_servo_driver = {
	.probe = pod_servo_drv_probe,
	.remove = pod_servo_drv_remove,
	.driver = {
		.name = DRIVER_NAME,
},
};

static int __init pod_servo_init(void)
{
	class_servo = class_create(THIS_MODULE, "pod_servo");
	if (IS_ERR(class_servo))
		return PTR_ERR(class_servo);

	return platform_driver_register(&pod_servo_driver);
}

static void __exit pod_servo_exit(void)
{
	platform_driver_unregister(&pod_servo_driver);
	class_destroy(class_servo);
}

module_init(pod_servo_init);
module_exit(pod_servo_exit);

MODULE_AUTHOR("Kevin JOLY, inspired by i.MX PWM driver");
MODULE_DESCRIPTION("POD servo driver");
MODULE_LICENSE("GPL");
