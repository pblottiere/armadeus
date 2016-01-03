/*
 * Platform data for IRQ manager generic driver
 *
 * (c) Copyright 2011    The Armadeus Project - ARMadeus Systems
 * Author: Julien Boibessot <julien.boibessot@armadeus.com>
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

#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#include <mach/hardware.h>
#ifdef CONFIG_MACH_APF9328 /* To remove when MX1 platform is merged */
# include "../arch/arm/plat-mxc/include/mach/fpga.h"
# include <mach/irqs.h>
#else
# include <mach/fpga.h>
#endif

#include "../../../virtual_components/irq_mngr/irq_mng.h"

static struct resource irq_mng0_resources[] = {
	[0] = {
		.start	= ARMADEUS_FPGA_BASE_ADDR + 0x0,
		.end	= ARMADEUS_FPGA_BASE_ADDR + 0x0 + (8 - 1),
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= ARMADEUS_FPGA_IRQ,
		.end	= ARMADEUS_FPGA_IRQ,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct ocore_irq_mng_pdata irq_mng0_data = {
	.num		= 0,
	.idnum		= 0x01,
	.idoffset	= 0x06,
};

static void irq_mng0_release(struct device *dev)
{
	dev_dbg(dev, "released\n");
}

static struct platform_device irq_mng0_device = {
	.name		= "ocore_irq_mng",
	.id		= 0,
	.dev		= {
		.release	= irq_mng0_release,
		.platform_data	= &irq_mng0_data
	},
	.num_resources	= ARRAY_SIZE(irq_mng0_resources),
	.resource	= irq_mng0_resources,
};

#ifdef CONFIG_MACH_APF27
static int fpga_pins[] = {
	(APF27_FPGA_INT_PIN | GPIO_IN | GPIO_GPIO),
};
#endif

static int __init board_irq_mng_init(void)
{
#ifdef CONFIG_MACH_APF27
	int ret;

	ret = mxc_gpio_setup_multiple_pins(fpga_pins, ARRAY_SIZE(fpga_pins), "FPGA");
	if (ret)
		return -EINVAL;
#endif
	set_irq_type(ARMADEUS_FPGA_IRQ, IRQ_TYPE_EDGE_RISING);

	return platform_device_register(&irq_mng0_device);
}

static void __exit board_irq_mng_exit(void)
{
	platform_device_unregister(&irq_mng0_device);
#ifdef CONFIG_MACH_APF27
	mxc_gpio_release_multiple_pins(fpga_pins, ARRAY_SIZE(fpga_pins));
#endif
}

module_init(board_irq_mng_init);
module_exit(board_irq_mng_exit);

MODULE_AUTHOR("Julien Boibessot <julien.boibessot@armadeus.com>");
MODULE_DESCRIPTION("Platform data for IRQ manager IP driver");
MODULE_LICENSE("GPL");
