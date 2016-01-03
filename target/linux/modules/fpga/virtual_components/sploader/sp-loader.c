/*
 * Generic Xilinx FPGA loader
 *
 * Copyright (C) 2006-2009 Julien Boibessot <julien.boibessot@armadeus.com>
 *                         Nicolas Colombain <nicolas.colombain@armadeus.com>
 *                         Armadeus Project / Armadeus systems
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

/* #define DEBUG */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

#include "spartan_loadsecond.h"

/* request_mem_region */
#include <linux/ioport.h>

#include <mach/fpga.h>
#include <asm/io.h>
#include "sploader.h"

#define ARMADEUS_FPGA_BASE_ADDR_VIRT 0xF4300000
#define DRIVER_VERSION		"0.91"
#define DRIVER_NAME		"sp_loader"
#define FPGA_PROC_DIRNAME	"driver/sp"
#define FPGA_PROC_FILENAME	FPGA_PROC_DIRNAME "/sploader"
#define FPGA_IOCTL 0x10000000	/* !! TO BE BETTER DEFINED !! */

struct sploader_dev {
	char *name;
	void *membase;
	struct resource *mem_res;
};

/* global variables */
Xilinx_desc *g_current_desc = NULL;
struct semaphore fpga_sema;

static unsigned char fpga_descriptor = 0;	/* use default target_fpga_desc */
module_param(fpga_descriptor, byte, 0);

#define FPGA_BUFFER_SIZE 4096
static unsigned char g_buffer[FPGA_BUFFER_SIZE];
static unsigned char g_nb_users = 0;

/* Handles write() done on /dev/fpga/loader */
static ssize_t armadeus_fpga_write(struct file *file, const char *pData,
				   size_t count, loff_t * f_pos)
{
	ssize_t ret = 0;

	/* Get exclusive access */
	if (down_interruptible(&fpga_sema))
		return -ERESTARTSYS;

	if (count > FPGA_BUFFER_SIZE) {
		count = FPGA_BUFFER_SIZE;
	}

	/* Get value to write from user space */
	ret = __copy_from_user(g_buffer, pData, count);
	if (ret != 0) {
		ret = -EFAULT;
		goto out;
	}

	ret = write_content(g_current_desc, g_buffer, count);

 out:
	/* Release exclusive access */
	up(&fpga_sema);

	return ret;
}

static int armadeus_fpga_open(struct inode *inode, struct file *file)
{
	int ret;
	u16 ip_id;

	/* Get exclusive access */
	if (down_interruptible(&fpga_sema))
		return -ERESTARTSYS;

	/* Only one access at a time is permitted */
	if (g_nb_users > 0) {
		ret = -EBUSY;
		goto out;
	}

	/* check if ID is correct */
	ip_id = readw(g_current_desc->ip_addr + g_current_desc->idoffset);
	if (ip_id != g_current_desc->idnum) {
		ret = -ENODEV;
		printk(KERN_WARNING "For %s id:%d doesn't match with "
						"id read %d,\n is device present ?\n",
						g_current_desc->name, g_current_desc->idnum, ip_id);
		goto out;
	}

	ret = loadsecond(g_current_desc);
	if (!ret) {
		printk("Starting FPGA download\n");
		g_nb_users++;
	}
	pr_debug("Opening /dev/sp_loader, %d %d\n", fpga_descriptor, ret);

 out:
	/* Release exclusive access */
	up(&fpga_sema);

	return ret;
}

static int armadeus_fpga_release(struct inode *inode, struct file *file)
{
	if (end_load(g_current_desc)) {
		printk("Failed to load FPGA !\n");
	}

	pr_debug("Closing access to /dev/fpgaloader\n");

	if (g_nb_users > 0)
		g_nb_users--;

	return 0;
}

/* PROC file */
static int procfile_fpga_read(char *buffer, __attribute__ ((unused))
			      char **start, off_t offset, int buffer_length,
			      int *eof, __attribute__ ((unused))
			      void *data)
{
	int ret;

	/* We give all of our information in one go, so if the user asks us if
	   we have more information the answer should always be no.
	   This is important because the standard read function from the
	   library would continue to issue the read system call until the
	   the kernel replies that it has no more information, or until 
	   its buffer is filled */
	if (offset > 0) {
		/* we have finished to read, return 0 */
		ret = 0;
	} else {
		//ret = fpga_get_infos(g_current_desc, buffer);
	}

	return ret;
}

static int procfile_fpga_write( __attribute__ ((unused))
			       struct file *file,
			       const char *buf, unsigned long count, void *data)
{
	return count;
}

/* Create /proc entries for direct access to FPGA config */
static int create_proc_entries(void)
{
	static struct proc_dir_entry *fpga_Proc_File;

	/* Create main directory */
	proc_mkdir(FPGA_PROC_DIRNAME, NULL);

	/* Create proc file */
	fpga_Proc_File =
	    create_proc_entry(FPGA_PROC_FILENAME,
			      S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, NULL);
	if (fpga_Proc_File == NULL) {
		printk(DRIVER_NAME ": Could not register a" FPGA_PROC_FILENAME
		       ". Terminating\n");
		goto error;
	} else {
		fpga_Proc_File->read_proc = procfile_fpga_read;
		fpga_Proc_File->write_proc = procfile_fpga_write;
	}

	return 0;
 error:
	remove_proc_entry(FPGA_PROC_DIRNAME, NULL);
	return -ENOMEM;
}

static struct file_operations fpga_fops = {
	.owner = THIS_MODULE,
	.write = armadeus_fpga_write,
	/*    .read    = armadeus_fpga_read, Configuration saving not supported yet */
	.open = armadeus_fpga_open,
	.release = armadeus_fpga_release,
};

static struct miscdevice fpgaloader_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DRIVER_NAME,
	.fops = &fpga_fops,
};

Xilinx_Spartan6_Slave_SelectMap_fns fpga_fns = {
	.pre = fpga_spartan6_pre_fn,
	.pgm = fpga_spartan6_pgm_fn,
	.init = fpga_spartan6_init_fn,
	.err = NULL,
	.done = fpga_spartan6_done_fn,
	.clk = fpga_spartan6_clk_fn,
	.cs = fpga_spartan6_cs_fn,
	.wr = fpga_spartan6_wr_fn,
	.rdata = fpga_spartan6_rdata_fn,
	.wdata = fpga_spartan6_wdata_fn,
	.busy = fpga_spartan6_busy_fn,
	.abort = fpga_spartan6_abort_fn,
	.post = fpga_spartan6_post_fn,
};

#ifdef CONFIG_PM
static int armadeus_sploader_suspend(struct platform_device *pdev,
				     pm_message_t state)
{
	return 0;
}

static int armadeus_sploader_resume(struct platform_device *pdev)
{
	return 0;
}
#else
#define armadeus_fpga_suspend   NULL
#define armadeus_fpga_resume    NULL
#endif

static int armadeus_sploader_probe(struct platform_device *pdev)
{
	Xilinx_desc *pdata = pdev->dev.platform_data;
	int ret = 0;
	int result;
	struct device *dev = &pdev->dev;
	Xilinx_desc *platform_info;
	struct resource *mem_res;
	struct sploader_dev *sdev;

	platform_info = dev->platform_data;
	if (platform_info == NULL) {
		dev_err(&pdev->dev, "probe - no platform data supplied\n");
		result = -ENODEV;
		goto err_no_pdata;
	}
	pdata->iface_fns = (void *)&fpga_fns;
	/* Register the driver through misc layer to get MAJOR/MINOR */
	result = misc_register(&fpgaloader_misc);
	if (result) {
		printk(KERN_ERR "%s:%u: misc_register failed %d\n",
		       __func__, __LINE__, result);
		goto err_no_pdata;
	}

	mem_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem_res) {
		dev_err(&pdev->dev, "can't find mem resource\n");
		return -EINVAL;
	}

	mem_res = request_mem_region(mem_res->start, resource_size(mem_res),
				     "sploader");
	if (!mem_res) {
		dev_err(&pdev->dev, "iomem already in use\n");
		return -EBUSY;
	}

	sdev = kmalloc(sizeof(struct sploader_dev), GFP_KERNEL);
	if (!sdev) {
		ret = -ENOMEM;
		goto out_release_mem;
	}

	platform_info->ip_addr =
	    ioremap(mem_res->start, resource_size(mem_res));
	if (!sdev->membase) {
		dev_err(&pdev->dev, "ioremap failed\n");
		ret = -ENOMEM;
		goto out_dev_free;
	}

	sdev->mem_res = mem_res;

	pdata->sdev = sdev;

	result = create_proc_entries();
	if (result < 0)
		goto out_iounmap;

	sema_init(&fpga_sema, 1);

	/* initialize the current fpga descriptor with the one by default */
	g_current_desc = platform_info;

	printk(DRIVER_NAME " v" DRIVER_VERSION " ready\n");

	return 0;

 out_iounmap:
	iounmap(platform_info->ip_addr);
 out_dev_free:
	kfree(sdev);
 out_release_mem:
	release_mem_region(mem_res->start, resource_size(mem_res));
 err_no_pdata:

	return ret;
}

static int armadeus_sploader_remove(struct platform_device *pdev)
{
	Xilinx_desc *dev = pdev->dev.platform_data;
	struct sploader_dev *sdev = (*dev).sdev;

	remove_proc_entry(FPGA_PROC_FILENAME, NULL);
	remove_proc_entry(FPGA_PROC_DIRNAME, NULL);
	misc_deregister(&fpgaloader_misc);
	iounmap(g_current_desc->ip_addr);
	kfree(sdev);
	release_mem_region(sdev->mem_res->start, resource_size(sdev->mem_res));
	printk(KERN_INFO "%s: deleted with success\n", dev->name);

	return 0;
}

static struct platform_driver plat_sploader_driver = {
	.probe = armadeus_sploader_probe,
	.remove = armadeus_sploader_remove,
	.suspend = armadeus_sploader_suspend,
	.resume = armadeus_sploader_resume,
	.driver = {
		   .name = "sploader",
		   .owner = THIS_MODULE,
		   },
};

static int __init armadeus_spartan6_init(void)
{
	int ret;

	printk("platform driver name %s\n", plat_sploader_driver.driver.name);
	ret = platform_driver_register(&plat_sploader_driver);
	return ret;
}

static void __exit armadeus_spartan6_exit(void)
{
	platform_driver_unregister(&plat_sploader_driver);
}

module_init(armadeus_spartan6_init);
module_exit(armadeus_spartan6_exit);

MODULE_AUTHOR("Julien Boibessot / Nicolas Colombain");
MODULE_AUTHOR("Gwenhael Goavec-Merou");
MODULE_DESCRIPTION("Armadeus SPVISION loading driver");
MODULE_LICENSE("GPL");
