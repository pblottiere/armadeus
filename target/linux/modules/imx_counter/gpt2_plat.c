/*
 * Copyright (c) 2010 Julien Boibessot <julien.boibessot@armadeus.com>
 * sponsored by Armadeus Systems.
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
#include <linux/clk.h>
#include <linux/platform_device.h>

#include <mach/common.h>
#include <mach/iomux-mx1-mx2.h>
#include <mach/gpio.h>

#include <../mach-mx2/devices.h>


struct clk *gpt2_clk;

static int apf27_gpt2_pins[] = {
	 (GPIO_PORTC | GPIO_IN  | GPIO_PF | 15),	/* TIN */
};

static int __init gpt_plat_probe(struct platform_device *pdev)
{
	int ret = 0;

	printk("--- %s\n", __func__);

	/* Activates clock */
	gpt2_clk = clk_get(&pdev->dev, "gpt_clk");
	
	/* Configure TIN */
	ret = mxc_gpio_setup_multiple_pins(apf27_gpt2_pins,
			ARRAY_SIZE(apf27_gpt2_pins), "GPT");

	return 0;
} 

static int __exit gpt_plat_remove(struct platform_device *pdev)
{
	printk("--- %s\n", __func__);

	mxc_gpio_release_multiple_pins(apf27_gpt2_pins, ARRAY_SIZE(apf27_gpt2_pins));

	return 0;
}


MODULE_ALIAS("platform:gpt_timer");

static struct platform_driver driver = {
	.driver = {
		.name = "imx_gpt",
		.owner = THIS_MODULE,
	},
	.probe = gpt_plat_probe,
	.remove = gpt_plat_remove,
};

static struct resource timer1_resources[] = {
	[0] = {
		.start  = GPT2_BASE_ADDR,
		.end    = GPT2_BASE_ADDR + 0x17,
		.flags  = IORESOURCE_MEM
	},
	[1] = {
		.start   = MXC_INT_GPT2,
		.end     = MXC_INT_GPT2,
		.flags   = IORESOURCE_IRQ,
        }
};

struct platform_device mxc_gpt1 = {
        .name = "imx_gpt",
        .id = 1,
        .num_resources = ARRAY_SIZE(timer1_resources),
	.resource = timer1_resources
};


static int __init gpt_plat_init(void)
{
	printk("--- %s\n", __func__);

	platform_driver_register(&driver);
	platform_device_register(&mxc_gpt1);
	
	return 0;
}

static void __exit gpt_plat_exit(void)
{
	printk("--- %s\n", __func__);
	/* To be completed ... */
}

module_init(gpt_plat_init);
module_exit(gpt_plat_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Julien Boibessot <julien.boibessot@armadeus.com>");
MODULE_DESCRIPTION("Platform data for APF27 GPT2");

