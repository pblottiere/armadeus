/*
 * APF27 Xilinx FPGA download support
 * Copyright (C) 2009 Nicolas Colombain <nicolas.colombain@armadeus.com>
 *                    Armadeus Project / Armadeus systems
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
 *
 */

#include <linux/version.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <asm/io.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#include <mach/iomux-mx1-mx2.h>
#else
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,20)
#include <mach/iomux-mx27.h>
#else
#include <linux/../../arch/arm/mach-imx/iomux-mx27.h>
#endif
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
#include <mach/gpio.h>
#else
#include <linux/gpio.h>
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,7,20)
#include <linux/armadeus_fpga.h>
#else
#include <mach/fpga.h>
#endif

#include "xilinx-fpga-loader.h"

#define CFG_FPGA_PWR		(GPIO_PORTF | 19)	/* FPGA prog pin  */
#define CFG_FPGA_PRG		(GPIO_PORTF | 11)	/* FPGA prog pin  */
#define CFG_FPGA_CLK		(GPIO_PORTF | 15)	/* FPGA clk pin   */
#define CFG_FPGA_INIT		(GPIO_PORTF | 12)	/* FPGA init pin  */
#define CFG_FPGA_DONE		(GPIO_PORTF |  9)	/* FPGA done pin  */
#define CFG_FPGA_RW		(GPIO_PORTF | 21)	/* FPGA done pin  */
#define CFG_FPGA_CS		(GPIO_PORTF | 22)	/* FPGA done pin  */
#define CFG_FPGA_SUSPEND	(GPIO_PORTF | 10)	/* FPGA done pin  */
#define CFG_FPGA_RESET		(GPIO_PORTF |  7)	/* FPGA done pin  */

void *fpga_membase;

static int fpga_shared_pins[] = {
	(CFG_FPGA_INIT | GPIO_IN | GPIO_PUEN | GPIO_GPIO),
};

/* Initialize GPIO port before download */
int apf27_fpga_pre(void)
{
	int res = 0;

	fpga_membase = ioremap(ARMADEUS_FPGA_BASE_ADDR, 16);
	if (!fpga_membase) {
		printk(KERN_ERR "ioremap failed\n");
		return -ENOMEM;
	}

	/* initialize common gpio "shared" with other apps */
	res =
	    mxc_gpio_setup_multiple_pins(fpga_shared_pins,
					 ARRAY_SIZE(fpga_shared_pins),
					 "FPGA_LOADER");
	if (res) {
		printk(KERN_ERR "FPGA prog pins already reserved !!\n");
		iounmap(fpga_membase);
		return res;
	}

	mxc_gpio_mode(CFG_FPGA_CLK | GPIO_OUT | GPIO_PUEN | GPIO_GPIO);
	mxc_gpio_mode(CFG_FPGA_RW | GPIO_OUT | GPIO_PUEN | GPIO_GPIO);
	mxc_gpio_mode(CFG_FPGA_CS | GPIO_OUT | GPIO_PUEN | GPIO_GPIO);

	/* power off fpga */
	gpio_set_value(CFG_FPGA_PWR, 1);
	mxc_gpio_mode(CFG_FPGA_PWR | GPIO_OUT | GPIO_PUEN | GPIO_GPIO);
	mdelay(10);

	gpio_set_value(CFG_FPGA_PRG, 1);
	gpio_set_value(CFG_FPGA_CLK, 1);
	gpio_set_value(CFG_FPGA_RW, 1);
	gpio_set_value(CFG_FPGA_CS, 1);
	gpio_set_value(CFG_FPGA_SUSPEND, 0);
	gpio_set_value(CFG_FPGA_RESET, 0);

	/* Initialize specific GPIO pins */
	mxc_gpio_mode(CFG_FPGA_DONE | GPIO_IN | GPIO_PUEN | GPIO_GPIO);
	mxc_gpio_mode(CFG_FPGA_PRG | GPIO_OUT | GPIO_PUEN | GPIO_GPIO);
	mxc_gpio_mode(CFG_FPGA_SUSPEND | GPIO_OUT | GPIO_PUEN | GPIO_GPIO);

	/* make sure the reset pin is active due to DLL start up */
	mxc_gpio_mode(CFG_FPGA_RESET | GPIO_OUT | GPIO_PUEN | GPIO_GPIO);
	gpio_set_value(CFG_FPGA_RESET, 1);

	/* make sure the fpga is powered */
	gpio_set_value(CFG_FPGA_PWR, 0);

	return res;
}

/*
 * Set the FPGA's active-low program line to the specified level
 */
int apf27_fpga_pgm(int assert)
{
	gpio_set_value(CFG_FPGA_PRG, !assert);

	return assert;
}

/*
 * Set the FPGA's active-high clock line to the specified level
 */
int apf27_fpga_clk(int assert_clk)
{
	gpio_set_value(CFG_FPGA_CLK, !assert_clk);

	return assert_clk;
}

/*
 * Test the state of the active-low FPGA INIT line.  Return 1 on INIT
 * asserted (low).
 */
int apf27_fpga_init(void)
{
	int value;

	value = gpio_get_value(CFG_FPGA_INIT);

	return !value;
}

/*
 * Test the state of the active-high FPGA DONE pin
 */
int apf27_fpga_done(void)
{
	return gpio_get_value(CFG_FPGA_DONE);
}

/*
 * Set the FPGA's wr line to the specified level
 */
int apf27_fpga_wr(int assert_write)
{
	gpio_set_value(CFG_FPGA_RW, !assert_write);

	return assert_write;
}

int apf27_fpga_cs(int assert_cs)
{
	gpio_set_value(CFG_FPGA_CS, !assert_cs);

	return assert_cs;
}

int apf27_fpga_wdata(unsigned char data)
{
	__raw_writew(data, fpga_membase);

	return data;
}

int apf27_fpga_busy(void)
{
	return 0;
}

int apf27_fpga_post(void)
{
	iounmap(fpga_membase);
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
	mxc_gpio_release_multiple_pins(fpga_shared_pins,
				       ARRAY_SIZE(fpga_shared_pins));
	/* see https://github.com/torvalds/linux/commit/581f84e24b45ab871ae5f15f9290261c7226ffa9 */
#endif
	/* reconfigure bus ctrl signals */
	mxc_gpio_mode(CFG_FPGA_RW | GPIO_PF | GPIO_PUEN);
	mxc_gpio_mode(CFG_FPGA_CS | GPIO_PF | GPIO_PUEN);
	mxc_gpio_mode(CFG_FPGA_CLK | GPIO_PF | GPIO_PUEN);
	/* end of prog */
	gpio_set_value(CFG_FPGA_PRG, 1);
	/* reset off */
	gpio_set_value(CFG_FPGA_RESET, 0);
	mxc_gpio_mode(CFG_FPGA_RESET | GPIO_OUT | GPIO_PUEN | GPIO_GPIO);

	return 0;
}

int apf27_fpga_abort(void)
{
	apf27_fpga_post();
	gpio_set_value(CFG_FPGA_PWR, 1);

	return 1;
}

/* Spartan2 code is used to download our Spartan 3: code is compatible.
 * Just take care about the file size
 */
Xilinx_Spartan3_Slave_Parallel_fns fpga_fns = {
	.pre = apf27_fpga_pre,
	.pgm = apf27_fpga_pgm,
	.clk = apf27_fpga_clk,
	.init = apf27_fpga_init,
	.done = apf27_fpga_done,
	.wr = apf27_fpga_wr,
	.cs = apf27_fpga_cs,
	.wdata = apf27_fpga_wdata,
	.busy = apf27_fpga_busy,
	.abort = apf27_fpga_abort,
	.post = apf27_fpga_post,
};

struct fpga_desc apf27_fpga_desc = {
	.family = Xilinx_Spartan,
	.iface = slave_parallel,
	.iface_fns = (void *)&fpga_fns
};

static struct platform_device fpga_device = {
	.name = "fpgaloader",
	.id = 0,
	.dev = {
		.platform_data = &apf27_fpga_desc,
	},
};

static struct platform_device *devices[] __initdata = {
	&fpga_device,
};

static int __init apf27_fpga_initialize(void)
{
	platform_add_devices(devices, ARRAY_SIZE(devices));

	return 0;
}

device_initcall(apf27_fpga_initialize);
