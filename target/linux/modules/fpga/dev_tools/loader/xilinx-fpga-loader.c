/*
 * Xilinx FPGAs download support
 * Copyright (C) 2006-2009 Nicolas Colombain <nicolas.colombain@armadeus.com>
 *                         Armadeus Project / Armadeus systems
 *
 * Based on the (U-Boot) implementation of:
 * Rich Ireland, Enterasys Networks, rireland@enterasys.com.
 * Keith Outwater, keith_outwater@mvis.com
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#include <asm/io.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,17,0)
#include <linux/time.h>
#else
#include <linux/timer.h>
#endif
#include <linux/delay.h>
#include <linux/errno.h>

#include "xilinx-fpga-loader.h"


#define CONFIG_FPGA_DELAY()
#define CFG_FPGA_WAIT   20000  /* uS */

size_t bytecount = 0; /* total bytes received */


/* Timeout function */
static unsigned long get_timer(unsigned long initTime)
{
	struct timeval tv;
	do_gettimeofday(&tv);
	if( tv.tv_usec > initTime )  /* avoid overflow pb */
		return tv.tv_usec - initTime;
	else
		return initTime - tv.tv_usec;
}

/* dump the given descriptor infos */
static int xilinx_dump_descriptor_info( struct fpga_desc *desc, char* buffer )
{
	int len;
	len  = sprintf( buffer, "%s %s\n",
			(desc->family == Xilinx_Spartan) ? "spartan":"unknown",
			(desc->iface == slave_serial) ? "slave serial":"slave parallel"
			);
	return len;
}

/**
 *  program the FPGA (serial mode).
 *  return 0 if success, >0 while programming, <0 if error detected
 */
static size_t spartan_serial_load(struct fpga_desc *desc, const char* buf, size_t bsize)
{
	Xilinx_Spartan_Slave_Serial_fns *fn = desc->iface_fns;

	if (fn) {
		int i;
		unsigned char   val;
		size_t nbbyte = 0; /* init local counter */

		while (nbbyte < bsize) {
			/* Spartan signals an error if INIT goes low (active)
			   while DONE is low (inactive) */
			if ((*fn->done)() == 0 && (*fn->init)()) {
				printk("** CRC error during FPGA load.\n");
				return -ETIMEDOUT;
			}
			val = buf[nbbyte ++];
			bytecount++;
			i = 8;
			do {
				/* Deassert the clock */
				(*fn->clk)(0);
				CONFIG_FPGA_DELAY();
				/* Write data */
				(*fn->wr)(val & 0x80);
				CONFIG_FPGA_DELAY();
				/* Assert the clock */
				(*fn->clk)(1);
				CONFIG_FPGA_DELAY();
				val <<= 1;
				i --;
			} while (i > 0);
		}

		if (bytecount % 4096 == 0)
			printk(".");

		return bsize;
	}
	return -EINVAL;
}

/**
 *  initialize the FPGA programming interface (serial).
 *  return 0 if success, <0 if error detected
 */
static int spartan_serial_init(struct fpga_desc *desc)
{
	Xilinx_Spartan_Slave_Serial_fns *fn = desc->iface_fns;
	if (fn) {
		unsigned long timestamp;

		if (*fn->pre){
			(*fn->pre)(); /* Run the pre configuration function if there is one. */
		}

		/* Establish the initial state */
		(*fn->pgm)(1);	/* Assert the program, commit */

		/* Wait for INIT state (init low) */
		timestamp = get_timer(0);		/* get current time */
		do {
			CONFIG_FPGA_DELAY ();
			if (get_timer(timestamp) > CFG_FPGA_WAIT) {
				pr_debug("** Timeout waiting for INIT to start.\n");
				return -ETIMEDOUT;
			}
		} while (!(*fn->init)());

		/* Get ready for the burn */
		CONFIG_FPGA_DELAY ();
		(*fn->pgm)(0);	/* Deassert the program, commit */

		timestamp = get_timer(0);		/* get current time */
		/* Now wait for INIT to go high */
		do {
			CONFIG_FPGA_DELAY ();
			if (get_timer(timestamp) > CFG_FPGA_WAIT) {
				pr_debug("** Timeout waiting for INIT to clear.\n");
				return -ETIMEDOUT;
			}
		} while ((*fn->init)());

		bytecount = 0; /* reset byte count */
		return 0; /* success */
	}
	return -EINVAL;
}

/* terminate the FPGA load (serial mode) */
static int spartan_serial_finish(struct fpga_desc *desc)
{
	unsigned long timestamp;
	Xilinx_Spartan_Slave_Serial_fns *fn = desc->iface_fns;

	if (fn) {
		CONFIG_FPGA_DELAY ();

		/* now check for done signal */
		timestamp = get_timer(0);	/* get current time */
		(*fn->wr)(1);

		while (! (*fn->done)()) {
			CONFIG_FPGA_DELAY ();
			(*fn->clk)(0);	/* Deassert the clock pin */
			CONFIG_FPGA_DELAY ();
			(*fn->clk)(1);	/* Assert the clock pin */

			if (get_timer(timestamp) > CFG_FPGA_WAIT) {
				pr_debug("** Timeout waiting for DONE.\n");
				return -ETIMEDOUT;
			}
		}
	}
	return 0;
}


/**
 *  initialize the FPGA programming interface (parallel).
 *  return 0 if success, <0 if error detected
 */
static int spartan_parallel_init(struct fpga_desc *desc)
{
	int res = 0;
	Xilinx_Spartan3_Slave_Parallel_fns *fn = desc->iface_fns;

	if (fn) {
		unsigned long timestamp;
		/*
		 * Run the pre configuration function if there is one.
		 */
		if (*fn->pre) {
			if( (res = (*fn->pre) ()) ) return res;
		}
		mdelay(100);

		/* Establish the initial state */
		(*fn->pgm) (1);	/* Assert the program, commit */
		timestamp = get_timer(0);		/* get current time */
		/* Now wait for INIT to go down */
		do {
			CONFIG_FPGA_DELAY ();
			if (get_timer(timestamp) > CFG_FPGA_WAIT) {
				pr_debug("** Timeout waiting for INIT to set.\n");
				(*fn->abort) ();	/* abort the burn */
				return -ETIMEDOUT;
			}
		} while (!(*fn->init) () );


		/* Get ready for the burn */
		CONFIG_FPGA_DELAY ();
		(*fn->pgm) (0);	/* Deassert the program, commit */

		timestamp = get_timer (0);		/* get current time */
		/* Now wait for INIT and BUSY to go high */
		do {
			CONFIG_FPGA_DELAY ();
			if (get_timer(timestamp) > CFG_FPGA_WAIT) {
				pr_debug("** Timeout waiting for INIT to clear.\n");
				(*fn->abort) ();	/* abort the burn */
				return -ETIMEDOUT;
			}
		} while ((*fn->init) ());
		(*fn->wr) (1); /* Assert write, commit */
		(*fn->cs) (1); /* Assert chip select, commit */
		(*fn->clk) (1);	/* Assert the clock pin */
		bytecount = 0; // reset byte count
	}
	return 0;
}

/**
 *  program the FPGA (parallel mode).
 *  return 0 if success, >0 while programming, <0 if error detected
 */
static size_t spartan_parallel_load(struct fpga_desc *desc, const char* buf, size_t bsize)
{
	unsigned long flags;
	Xilinx_Spartan3_Slave_Parallel_fns *fn = desc->iface_fns;

	if (fn) {
		/* Load the data */
		size_t nbbyte = 0;

		raw_local_irq_save(flags);
		 while (nbbyte < bsize) {
			(*fn->clk) (1);	/* Assert the clock pin */
			CONFIG_FPGA_DELAY ();
 			(*fn->wdata) (buf[nbbyte++]); /* write the data */
			CONFIG_FPGA_DELAY ();
			(*fn->clk) (0);	/* Deassert the clock pin */
			bytecount++;
		}
		raw_local_irq_restore(flags);

		return bsize;
	}
	return -EINVAL;
}

/* terminate the FPGA load (parallel mode) */
static int spartan_parallel_finish(struct fpga_desc *desc)
{
	Xilinx_Spartan3_Slave_Parallel_fns *fn = desc->iface_fns;
	unsigned long timestamp;

	if (fn) {
		/* now check for done signal */
		timestamp = get_timer(0);	/* get current time */
		while (!(*fn->done) ()) {
			CONFIG_FPGA_DELAY ();
			(*fn->clk) (0);	/* Deassert the clock pin */
			CONFIG_FPGA_DELAY ();
			(*fn->clk) (1);	/* Assert the clock pin */

			if (get_timer(timestamp) > CFG_FPGA_WAIT) {
				printk("** Timeout waiting for DONE to clear. %d bytes\n", bytecount);
				(*fn->abort) ();	/* abort the burn */
				return -ETIMEDOUT;
			}
		}
		(*fn->cs) (0);	/* Deassert the chip select */
		(*fn->wr) (0);	/* Deassert the write pin */
		(*fn->post) (); /* release resources */
	}

	return 0;
}


size_t fpga_load( struct fpga_desc *desc, const char *buf, size_t bsize )
{
	int ret = 0;

	if (desc) {
		switch (desc->family) /* check family */
		{
			case Xilinx_Spartan:
			{
				switch (desc->iface) /* check download hardware interface */
				{
					case slave_serial:
						ret = spartan_serial_load(desc, buf, bsize);
					break;
					case slave_parallel:
						ret = spartan_parallel_load (desc, buf, bsize);
					break;
					default:
						PRINTF("interface not supported!\n");
						ret = -ENOSYS;
					break;
				}
			}
			break;

			default:
				PRINTF("family not supported!\n");
				ret = -ENOSYS;
			break;
		}
	}
	else {
		PRINTF("invalid FPGA descriptor!\n");
		ret = -EINVAL;
	}
	return ret;
}

int fpga_init_load( struct fpga_desc *desc )
{
	int ret = 0;

	if (desc) {
		switch (desc->family)
		{
			case Xilinx_Spartan:
			{
				switch (desc->iface) /* check donwload hardware interface */
				{
					case slave_serial:
						ret = spartan_serial_init( desc );
					break;
					case slave_parallel:
						ret = spartan_parallel_init( desc );
					break;
					default:
						pr_debug("interface not supported!\n");
						ret = -ENOSYS;
					break;
				}
			}
			break;
			default:
				pr_debug("family not supported!\n");
				ret = -ENOSYS;
			break;
		}
	}
	else {
		pr_debug("invalid FPGA descriptor!\n");
		ret = -EINVAL;
	}
	return ret;
}

int fpga_finish_load(struct fpga_desc *desc)
{
	int ret = 0;

	if (desc) {
		switch (desc->family)
		{
		case Xilinx_Spartan:
		{
			switch (desc->iface) /* check hardware download interface */
			{
				case slave_serial:
					ret = spartan_serial_finish(desc);
				break;
				case slave_parallel: 
					ret = spartan_parallel_finish(desc);
				break;
				default:
					pr_debug("interface not supported!\n");
					ret = -ENOSYS;
				break;
			}
		}
		break;
		default:
			pr_debug("family not supported!\n");
			ret = -ENOSYS;
		break;
		}
	} else {
		pr_debug("invalid FPGA descriptor!\n");
		ret = -EINVAL;
	}

	return ret;
}

int fpga_get_infos( struct fpga_desc *desc, char* buffer )
{
	int len = 0;

	if (desc) {
		len  = xilinx_dump_descriptor_info( desc, buffer );
	}

	return len;
}
