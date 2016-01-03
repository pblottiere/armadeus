/*
 * altera_cvp.c -- driver for configuring Altera FPGAs via CvP
 *
 * Written by: Andres Cassinelli <acassine@altera.com>
 *             Altera Corporation
 *
 * Copyright (C) 2012 Altera Corporation. All Rights Reserved.
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license. The text of
 * the BSD license is provided below.
 *
 * BSD License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this software
 * may be distributed under the terms of the GNU General Public License ("GPL")
 * version 2, in which case the provisions of the GPL apply INSTEAD OF those
 * given above. A copy of the GPL may be found in the file GPLv2.txt provided
 * with this distribution in the same directory as this file.
 *
 * THIS SOFTWARE IS PROVIDED BY ALTERA CORPORATION "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/device.h> /* dev_err(), etc. */
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/uaccess.h> /* copy_to/from_user */
#include <linux/slab.h>  /* kmalloc */
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include "altera_cvp.h"

/* PCIe Vendor & Device IDs are parameters passed to the module when it's loaded */
static unsigned short vid = 0x1172; /* default to Altera's Vendor ID */
static unsigned short did = 0xE001; /* default to PCIe Reference Design Device ID */
module_param(vid, ushort, S_IRUGO);
module_param(did, ushort, S_IRUGO);

struct altera_cvp_dev cvp_dev; /* contents initialized in altera_cvp_init() */
static unsigned int altera_cvp_major; /* major number to use */

/* CvP helper functions */

static int altera_cvp_get_offset_and_mask(int bit, int *byte_offset, u8 *mask)
{
	switch (bit) {
		case DATA_ENCRYPTED:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_STATUS;
			*mask = MASK_DATA_ENCRYPTED;
			break;
		case DATA_COMPRESSED:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_STATUS;
			*mask = MASK_DATA_COMPRESSED;
			break;
		case CVP_CONFIG_READY:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_STATUS;
			*mask = MASK_CVP_CONFIG_READY;
			break;
		case CVP_CONFIG_ERROR:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_STATUS;
			*mask = MASK_CVP_CONFIG_ERROR;
			break;
		case CVP_EN:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_STATUS;
			*mask = MASK_CVP_EN;
			break;
		case USER_MODE:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_STATUS;
			*mask =  MASK_USER_MODE;
			break;
		case PLD_CLK_IN_USE:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_STATUS + 1;
			*mask = MASK_PLD_CLK_IN_USE;
			break;
		case CVP_MODE:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_MODE_CTRL;
			*mask = MASK_CVP_MODE;
			break;
		case HIP_CLK_SEL:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_MODE_CTRL;
			*mask = MASK_HIP_CLK_SEL;
			break;
		case CVP_CONFIG:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_PROG_CTRL;
			*mask = MASK_CVP_CONFIG;
			break;
		case START_XFER:
			*byte_offset = OFFSET_VSEC + OFFSET_CVP_PROG_CTRL;
			*mask = MASK_START_XFER;
			break;
		case CVP_CFG_ERR_LATCH:
			*byte_offset = OFFSET_VSEC + OFFSET_UNC_IE_STATUS;
			*mask = MASK_CVP_CFG_ERR_LATCH;
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

static int altera_cvp_read_bit(int bit, u8 *value)
{
	int byte_offset;
	u8 byte_val, byte_mask;
	if (altera_cvp_get_offset_and_mask(bit, &byte_offset, &byte_mask))
		return -EINVAL;
	if (pci_read_config_byte(cvp_dev.pci_dev, byte_offset, &byte_val))
		return -EAGAIN;
	*value = (byte_val & byte_mask) ? 1 : 0;
	return 0;
}

static int altera_cvp_write_bit(int bit, u8 value)
{
	int byte_offset;
	u8 byte_val, byte_mask;

	switch (bit) {
		case CVP_MODE:
		case HIP_CLK_SEL:
		case CVP_CONFIG:
		case START_XFER:
		case CVP_CFG_ERR_LATCH:
			if(altera_cvp_get_offset_and_mask(bit, &byte_offset, &byte_mask))
				return -EINVAL;
			pci_read_config_byte(cvp_dev.pci_dev, byte_offset, &byte_val);
			byte_val = value ? (byte_val | byte_mask) : (byte_val & ~byte_mask);
			pci_write_config_byte(cvp_dev.pci_dev, byte_offset, byte_val);
			return 0;
		default:
			return -EINVAL; /* only the bits above are writeable */
	}
}

static int altera_cvp_set_num_clks(int num_clks)
{
	if (num_clks < 1 || num_clks > 64)
		return -EINVAL;
	if (num_clks == 64)
		num_clks = 0x00;
	return (pci_write_config_byte(cvp_dev.pci_dev,
					OFFSET_VSEC+OFFSET_CVP_NUMCLKS,
					num_clks));
}

#define NUM_REG_WRITES 244
#define DUMMY_VALUE 0x00000000
/**
 * altera_cvp_switch_clk() - switch between CvP clock and internal clock
 *
 * Issues dummy memory writes to the PCIe HIP, allowing the Control Block to
 * switch between the HIP's CvP clock and the internal clock.
 */
static int altera_cvp_switch_clk(void)
{
	int i;
	altera_cvp_set_num_clks(1);
	for (i = 0; i < NUM_REG_WRITES; i++) {
		iowrite32(DUMMY_VALUE, cvp_dev.wr_addr);
	}
	return 0;
}

static int altera_cvp_set_data_type(void)
{
	int error, num_clks;
	u8 compr, encr;

	if ((error = altera_cvp_read_bit(DATA_COMPRESSED, &compr)) ||
	    (error = altera_cvp_read_bit(DATA_ENCRYPTED, &encr)))
		return error;

	if (compr)
		num_clks = 8;
	else if (encr)
		num_clks = 4;
	else
		num_clks = 1;

	return (altera_cvp_set_num_clks(num_clks));
}

static int altera_cvp_send_data(u32 *data, unsigned long num_words)
{
#ifdef DEBUG
	u8 bit_val;
	unsigned int i;
	for (i = 0; i < num_words; i++) {
		iowrite32(data[i], cvp_dev.wr_addr);
		if (i + 1 % ERR_CHK_INTERVAL == 0) {
			altera_cvp_read_bit(CVP_CONFIG_ERROR, &bit_val);
			if (bit_val) {
				dev_err(&cvp_dev.pci_dev->dev, "CB detected a CRC error "
								"between words %d and %d\n",
								i + 1 - ERR_CHK_INTERVAL,
								i + 1);
				return -EAGAIN;
			}
		}
	}
	dev_info(&cvp_dev.pci_dev->dev, "A total of %ld 32-bit words were "
					"sent to the FPGA\n", num_words);
#else
	iowrite32_rep(cvp_dev.wr_addr, data, num_words);
#endif /* DEBUG */
	return 0;
}

/* Polls the requested bit until it has the specified value (or until timeout) */
/* Returns 0 once the bit has that value, error code on timeout */
static int altera_cvp_wait_for_bit(int bit, u8 value)
{
	u8 bit_val;
	static DECLARE_WAIT_QUEUE_HEAD(cvp_wq);

	altera_cvp_read_bit(bit, &bit_val);
	if (bit_val != value) {
		wait_event_timeout(cvp_wq, 0, MAX_WAIT);
		altera_cvp_read_bit(bit, &bit_val);
		if (bit_val != value) {
			dev_info(&cvp_dev.pci_dev->dev, "Timed out while "
							"polling bit %d\n", bit);
			return -EAGAIN;
		}
	}
	return 0;
}

static int altera_cvp_setup(void)
{
	if(altera_cvp_write_bit(HIP_CLK_SEL, 1))
		return -EINVAL;
	if(altera_cvp_write_bit(CVP_MODE, 1))
		return -EINVAL;
	altera_cvp_switch_clk(); /* allow CB to sense if system reset is issued */
	if(altera_cvp_write_bit(CVP_CONFIG, 1)) /* request CB to begin CvP transfer */
		return -EINVAL;

	if (altera_cvp_wait_for_bit(CVP_CONFIG_READY, 1)) /* wait until CB is ready */
		return -EAGAIN;

	altera_cvp_switch_clk();
	if(altera_cvp_write_bit(START_XFER, 1))
		return -EINVAL;
	altera_cvp_set_data_type();
	dev_info(&cvp_dev.pci_dev->dev, "Now starting CvP...\n");
	return 0; /* success */
}

static int altera_cvp_teardown(void)
{
	u8 bit_val;

	/* if necessary, flush remainder buffer */
	if (cvp_dev.remain_size > 0) {
		u32 last_word = 0;
		memcpy(&last_word, cvp_dev.remain, cvp_dev.remain_size);
		altera_cvp_send_data(&last_word, cvp_dev.remain_size);
	}

	if(altera_cvp_write_bit(START_XFER, 0))
		return -EINVAL;
	if(altera_cvp_write_bit(CVP_CONFIG, 0)) /* request CB to end CvP transfer */
		return -EINVAL;
	altera_cvp_switch_clk();

	if (altera_cvp_wait_for_bit(CVP_CONFIG_READY, 0)) /* wait until CB is ready */
		return -EAGAIN;

	altera_cvp_read_bit(CVP_CFG_ERR_LATCH, &bit_val);
	if (bit_val) {
		dev_err(&cvp_dev.pci_dev->dev, "Configuration error detected, "
						"CvP has failed\n");
		if(altera_cvp_write_bit(CVP_CFG_ERR_LATCH, 1)) /* clear error bit */
			return -EINVAL;
	}

	if(altera_cvp_write_bit(CVP_MODE, 0))
		return -EINVAL;
	if(altera_cvp_write_bit(HIP_CLK_SEL, 0))
		return -EINVAL;

	if (!bit_val) { /* wait for application layer to be ready */
		if(altera_cvp_wait_for_bit(PLD_CLK_IN_USE, 1))
			return -EAGAIN;
		if(altera_cvp_wait_for_bit(USER_MODE, 1))
			return -EAGAIN;
		dev_info(&cvp_dev.pci_dev->dev, "CvP successful, application "
						"layer now ready\n");
	}

	return 0; /* success */
}

/* Open and close */

int altera_cvp_open(struct inode *inode, struct file *filp)
{
	/* enforce single-open */
	if (!atomic_dec_and_test(&cvp_dev.is_available)) {
		atomic_inc(&cvp_dev.is_available);
		return -EBUSY;
	}

	if ((filp->f_flags & O_ACCMODE) != O_RDONLY) {
		u8 cvp_enabled = 0;
		if (altera_cvp_read_bit(CVP_EN, &cvp_enabled))
			return -EAGAIN;
		if (cvp_enabled) {
			return altera_cvp_setup();
		} else {
			dev_err(&cvp_dev.pci_dev->dev, "CvP is not enabled in "
							"the design on this "
							"FPGA\n");
			return -EOPNOTSUPP;
		}
	}
	return 0; /* success */
}

int altera_cvp_release(struct inode *inode, struct file *filp)
{
	atomic_inc(&cvp_dev.is_available); /* release the device */
	if ((filp->f_flags & O_ACCMODE) != O_RDONLY) {
		return altera_cvp_teardown();
	}
	return 0; /* success */
}

/* Read and write */

ssize_t altera_cvp_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int dev_size = NUM_VSEC_REGS * BYTES_IN_REG;
	int i, byte_offset;
	u8 *out_buf;
	ssize_t ret_val; /* number of bytes successfully read */

	if (*f_pos >= dev_size)
		return 0; /* we're at EOF already */
	if (*f_pos + count > dev_size)
		count = dev_size - *f_pos; /* we can only read until EOF */

	out_buf = kmalloc(count, GFP_KERNEL);

	for (i = 0; i < count; i++) {
		byte_offset = OFFSET_VSEC + *f_pos + i;
		pci_read_config_byte(cvp_dev.pci_dev, byte_offset, &out_buf[i]);
	}

	if (copy_to_user(buf, out_buf, count)) {
		ret_val = -EFAULT;
	} else {
		*f_pos += count;
		ret_val = count;
	}

	kfree(out_buf);
	return ret_val;
}

ssize_t altera_cvp_write(struct file * filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret_val; /* number of bytes successfully transferred */
	u8 *send_buf;
	size_t send_buf_size;

	send_buf_size = count + cvp_dev.remain_size;
	send_buf = kmalloc(send_buf_size, GFP_KERNEL);

	if (cvp_dev.remain_size > 0)
		memcpy(send_buf, cvp_dev.remain, cvp_dev.remain_size);

	if (copy_from_user(send_buf + cvp_dev.remain_size, buf, count)) {
		ret_val = -EFAULT;
		goto exit;
	}

	/* calculate new remainder */
	cvp_dev.remain_size = send_buf_size % 4;

	/* save bytes in new remainder in cvp_dev */
	if (cvp_dev.remain_size > 0)
		memcpy(cvp_dev.remain,
			send_buf + (send_buf_size - cvp_dev.remain_size),
			cvp_dev.remain_size);

	if (altera_cvp_send_data((u32 *)send_buf, send_buf_size / 4)) {
		ret_val = -EAGAIN;
		goto exit;
	}

	*f_pos += count;
	ret_val = count;
exit:
	kfree (send_buf);
	return ret_val;
}

struct file_operations altera_cvp_fops = {
	.owner =   THIS_MODULE,
	.llseek =  no_llseek,
	.read =    altera_cvp_read,
	.write =   altera_cvp_write,
	.open =    altera_cvp_open,
	.release = altera_cvp_release,
};

/* PCI functions */

static int altera_cvp_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	int rc = 0;

	if((dev->vendor == vid) && (dev->device == did)) {
		rc = pci_enable_device(dev);
		if (rc) {
			dev_err(&dev->dev, "pci_enable device() failed\n");
			return rc;
		}
		dev_info(&dev->dev, "Found and enabled PCI device with "
					"VID 0x%04X, DID 0x%04X\n", vid, did);

		rc = pci_request_regions(dev, ALTERA_CVP_DRIVER_NAME);
		if (rc) {
			dev_err(&dev->dev, "pci_request_regions() failed\n");
			return rc;
		}

		cvp_dev.wr_addr = pci_iomap(dev, 0, 0);

		cvp_dev.pci_dev = dev; /* store pointer for PCI API calls */
		return 0;
	} else {
		dev_err(&dev->dev, "This PCI device does not match "
					"VID 0x%04X, DID 0x%04X\n", vid, did);
		return -ENODEV;
	}
}

static void altera_cvp_remove(struct pci_dev *dev)
{
	pci_iounmap(dev, cvp_dev.wr_addr);
	pci_disable_device(dev);
	pci_release_regions(dev);
}

/* PCIe HIP on FPGA can have any combination of IDs based on design settings */
static struct pci_device_id pci_ids[] = {
	{ PCI_DEVICE(PCI_ANY_ID, PCI_ANY_ID), },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static struct pci_driver cvp_driver = {
	.name = ALTERA_CVP_DRIVER_NAME,
	.id_table = pci_ids,
	.probe = altera_cvp_probe,
	.remove = altera_cvp_remove,
};

/* Module functions */

static int __init altera_cvp_init(void)
{
	int rc = 0;
	dev_t dev;

	rc = alloc_chrdev_region(&dev, 0, 1, ALTERA_CVP_DEVFILE);
	if (rc) {
		printk(KERN_ERR "%s: Allocation of char device numbers "
					"failed\n", ALTERA_CVP_DRIVER_NAME);
		goto exit;
	}

	cvp_dev.dev = dev; /* store major and minor numbers for char device */
	altera_cvp_major = MAJOR(dev);

	rc = pci_register_driver(&cvp_driver);
	if (rc) {
		printk(KERN_ERR "%s: PCI driver registration failed\n",
					ALTERA_CVP_DRIVER_NAME);
		unregister_chrdev_region(MKDEV(altera_cvp_major, 0), 1);
		goto exit;
	}

	cdev_init(&cvp_dev.cdev, &altera_cvp_fops);
	cvp_dev.cdev.owner = THIS_MODULE;
	rc = cdev_add(&cvp_dev.cdev, dev, 1);
	if (rc) {
		printk(KERN_ERR "%s: Unable to add char device to the "
					"system\n", ALTERA_CVP_DRIVER_NAME);
		pci_unregister_driver(&cvp_driver);
		goto exit;
	}

	cvp_dev.remain_size = 0;
	atomic_set(&cvp_dev.is_available, 1);
	printk("successfully init Altera CVP with major %d\n", altera_cvp_major);
exit:
	return rc;
}

static void __exit altera_cvp_exit(void)
{
	cdev_del(&cvp_dev.cdev);
	unregister_chrdev_region(MKDEV(altera_cvp_major, 0), 1);
	pci_unregister_driver(&cvp_driver);
}

module_init(altera_cvp_init);
module_exit(altera_cvp_exit);

MODULE_AUTHOR("Andres Cassinelli <acassine@altera.com>");
MODULE_DESCRIPTION("Configuration driver for Altera CvP-capable FPGAs");
MODULE_VERSION(ALTERA_CVP_DRIVER_VERSION);
MODULE_LICENSE("Dual BSD/GPL");

