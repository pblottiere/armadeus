/*
 ***********************************************************************
 *
 * (c) Copyright 2011	Armadeus project
 * Fabien Marteau <fabien.marteau@armadeus.com>
 * Gwenhael Goavec-Merou <gwenhael.goavec-merou@armadeus.com>
 * loader driver for generic sploader driver
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
 **********************************************************************
 */

#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

# include <mach/fpga.h>

#include "../../virtual_components/sploader/spartan_loadsecond.h"

static struct resource sploader0_resources[] = {
	[0] = {
		.start	= ARMADEUS_FPGA_BASE_ADDR, + 0x0,
		.end	= ARMADEUS_FPGA_BASE_ADDR + 0x0 + 3,
		.flags	= IORESOURCE_MEM,
	},
};

static Xilinx_desc plat_sploader0_data = {
	.family = Xilinx_Spartan6,
	.iface = slave_parallel,
	.size = (11875104l / 8),    //XILINX_XC6SLX45_SIZE,
	.fpga_offset = ARMADEUS_FPGA_BASE_ADDR,
	.cookie = 1,
	.name		= "SP_VISION_CONFIGURE00",
	.num		= 0,
	.idnum		= 1,
	.idoffset	=  0x0 * (16 /8)
};


void plat_sploader_release(struct device *dev)
{
	dev_dbg(dev, "released\n");
}

static struct platform_device plat_sploader_devices[] = {
    {
	    .name	= "sploader",
	    .id	= 0,
	    .dev	= {
	    	.release	= plat_sploader_release,
	    	.platform_data	= &plat_sploader0_data
	    },
	    .num_resources	= ARRAY_SIZE(sploader0_resources),
	    .resource	= sploader0_resources,
    }

};

static int __init sploader_init(void)
{
	return platform_device_register(plat_sploader_devices);
}

static void __exit sploader_exit(void)
{
	platform_device_unregister(plat_sploader_devices);
}

module_init(sploader_init);
module_exit(sploader_exit);

MODULE_AUTHOR("Fabien Marteau <fabien.marteau@armadeus.com>");
MODULE_AUTHOR("Gwenhael Goavec-Merou <gwenhael.goavec-merou@armadeus.com>");
MODULE_DESCRIPTION("Driver to load spvision fpga");
MODULE_LICENSE("GPL");

