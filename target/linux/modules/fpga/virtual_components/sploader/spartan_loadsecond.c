#include <linux/types.h>
#include "spartan_loadsecond.h"
#include <asm/io.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/errno.h>

#define FPGA_FAIL -1
#define FPGA_SUCCESS 0
#define FALSE                    (0)
#define TRUE                     (!FALSE)

#define CONFIG_SYS_FPGA_WAIT        20000	/* 20 ms */
#define CONFIG_FPGA_DELAY()

#define SELECTMAP_CONFIG_RDWR_N     0
#define SELECTMAP_CONFIG_PROGRAM_N  1
#define SELECTMAP_CONFIG_CSI_N      2
#define SELECTMAP_CONFIG_OS         3
#define SELECTMAP_CONFIG_CLK        4
#define SELECTMAP_CONFIG_REG        (desc->ip_addr+2)

#define SELECTMAP_STATUS_DONE       0
#define SELECTMAP_STATUS_BUSY       1
#define SELECTMAP_STATUS_INIT_N     2
#define SELECTMAP_STATUS_REG        (desc->ip_addr+4)
#define SELECTMAP_DATA_REG          (desc->ip_addr+6)

Xilinx_desc *xil_desc;

static unsigned long get_timer(unsigned long initTime)
{
	struct timeval tv;
	do_gettimeofday(&tv);
	if (tv.tv_usec > initTime)	/* avoid overflow pb */
		return tv.tv_usec - initTime;
	else
		return initTime - tv.tv_usec;
}

int fpga_spartan6_abort_fn(Xilinx_desc *desc, int cookie)
{
	int config;

	config = readw(SELECTMAP_CONFIG_REG);	/* read config */
	config |= (1 << SELECTMAP_CONFIG_CLK);	/* IP in clock mode */
	writew(config, SELECTMAP_CONFIG_REG);	/* write config */

	return cookie;
}

int fpga_spartan6_busy_fn(Xilinx_desc *desc, int cookie)
{
	u16 status;

	status = readw(SELECTMAP_STATUS_REG);	/* read config */

	return (((status & (1 << SELECTMAP_STATUS_BUSY)) != 0) ? 0 : 1);
}

int fpga_spartan6_post_fn(Xilinx_desc *desc, int cookie)
{
	int config;

	config = readw(SELECTMAP_CONFIG_REG);	/* read config */
	config |= (1 << SELECTMAP_CONFIG_CLK);	/* IP in clock mode */
	config &= ~(1 << SELECTMAP_CONFIG_OS);	/* OS High */
	writew(config, SELECTMAP_CONFIG_REG);	/* write config */

	return cookie;
}

int loadsecond(Xilinx_desc * desc)
{
	int ret_val;		// = FPGA_FAIL;     /* assume the worst */
	Xilinx_Spartan6_Slave_SelectMap_fns *fn;
	xil_desc = desc;
	fn = desc->iface_fns;
	ret_val = FPGA_FAIL;	/* assume the worst */

	if (fn) {
		int ret;
		int cookie = desc->cookie;	/* make a local copy */
		unsigned long ts;	/* timestamp */

		/*
		 * This code is designed to emulate the "Express Style"
		 * Continuous Data Loading in Slave Parallel Mode for
		 * the Spartan-VI Family.
		 *
		 * Run the pre configuration function if there is one.
		 */
		if (*fn->pre) {
			ret = (*fn->pre) (desc, cookie);
			if (ret < 0)
				return FPGA_FAIL;
		}

		/* PROG_N low */
		(*fn->pgm) (desc, TRUE, TRUE, cookie);	/* Assert the program, commit */

		CONFIG_FPGA_DELAY();

		/* PROG_N high */
		(*fn->pgm) (desc, FALSE, TRUE, cookie);	/* deAssert the program, commit */

		/* Wait for INIT_N low */
		ts = get_timer(0);	/* get current time */
		do {
			CONFIG_FPGA_DELAY();

			if (get_timer(ts) > CONFIG_SYS_FPGA_WAIT) {	/* check the time */
				printk ("** Timeout waiting for INIT to clear.\n");
				(*fn->abort) (desc, cookie);	/* abort the burn */
				return FPGA_FAIL;
			}
		} while ((*fn->init) (desc, cookie) == FPGA_SUCCESS);

		(*fn->wr) (desc, TRUE, TRUE, cookie);	/* Assert write, commit */
		(*fn->cs) (desc, TRUE, TRUE, cookie);	/* Assert chip select, commit */
	}
	return 0;
}

int write_content(Xilinx_desc * desc, void *buf, size_t bsize)
{
	unsigned long flags;
	Xilinx_Spartan6_Slave_SelectMap_fns *fn = desc->iface_fns;

	if (fn) {
		int cookie = desc->cookie;	/* make a local copy */
		size_t bytecount = 0;
		unsigned char *data = (unsigned char *)buf;
		/* Load the data */
		raw_local_irq_save(flags);
		while (bytecount < bsize) {
			(*fn->wdata) (desc, data[bytecount], TRUE, cookie);	/* write the data */
			bytecount++;
		}
		raw_local_irq_restore(flags);
		return bsize;
	}
	return -EINVAL;
}

int end_load(Xilinx_desc * desc)
{
	int ret_val = FPGA_FAIL;	/* assume the worst */
	Xilinx_Spartan6_Slave_SelectMap_fns *fn;
	fn = desc->iface_fns;
	ret_val = FPGA_FAIL;	/* assume the worst */

	if (fn) {
		int cookie = desc->cookie;	/* make a local copy */
		unsigned long ts;	/* timestamp */

		/* now check for done signal */
		ts = get_timer(0);	/* get current time */
		ret_val = FPGA_SUCCESS;
		while ((*fn->done) (desc, cookie) == FPGA_FAIL) {
			/* XXX - we should have a check in here somewhere to
			 * make sure we aren't busy forever... */

			if (get_timer(ts) > 20000 ) {	/* check the time */
				printk("** Timeout waiting for DONE to clear.\n");
				(*fn->abort) (desc, cookie);	/* abort the burn */
				ret_val = FPGA_FAIL;
				break;
			}
		}

		CONFIG_FPGA_DELAY();
		(*fn->cs) (desc, FALSE, TRUE, cookie);	/* Deassert the chip select */
		(*fn->wr) (desc, FALSE, TRUE, cookie);	/* Deassert the write pin */

		printk("\n");	/* terminate the dotted line */

		if (ret_val == FPGA_SUCCESS) {
			printk("Done.\n");
		}
		/*
		 * Run the post configuration function if there is one.
		 */
		if (*fn->post) {
			(*fn->post) (desc, cookie);
		} else {
			printk("Fail.\n");
		}
	} else {
		printk("%s: NULL Interface function table!\n", __FUNCTION__);
	}
	return ret_val;
}

int fpga_spartan6_init_fn(Xilinx_desc *desc, int cookie)
{
	uint16_t status;

	status = readw(SELECTMAP_STATUS_REG);

	return (((status & (1 << SELECTMAP_STATUS_INIT_N)) ==
		 0) ? FPGA_SUCCESS : FPGA_FAIL);
}

/* Set the FPGA's active-high clock line to the specified level */
int fpga_spartan6_clk_fn(Xilinx_desc *desc, int assert_clk, int flush, int cookie)
{
	/* clock pulse is generated when a data is wrote */
	return assert_clk;
}

/* Test the state of the active-high FPGA DONE pin */
int fpga_spartan6_done_fn(Xilinx_desc *desc, int cookie)
{
	uint16_t status;

	status = readw(SELECTMAP_STATUS_REG);
	return (((status & (1 << SELECTMAP_STATUS_DONE)) ==
		 0) ? FPGA_FAIL : FPGA_SUCCESS);
}

/* TODO: read/write in 16bits mode */
int fpga_spartan6_rdata_fn(Xilinx_desc *desc, unsigned char *data, int cookie)
{
	*data = (readw(SELECTMAP_DATA_REG) & 0xff);
	printk(".");

	return *data;
}

int fpga_spartan6_wdata_fn(Xilinx_desc *desc, unsigned char data, 
							int flush, int cookie)
{
	uint16_t val = data & 0x00ff;

	writew(val, SELECTMAP_DATA_REG);

	return data;
}

/* Set the FPGA's wr line to the specified level */
int fpga_spartan6_wr_fn(Xilinx_desc *desc, int assert_write, int flush, int cookie)
{
	unsigned short config;

	config = readw(SELECTMAP_CONFIG_REG);
	if (assert_write)
		config &= ~(1 << SELECTMAP_CONFIG_RDWR_N);
	else
		config |= (1 << SELECTMAP_CONFIG_RDWR_N);
	writew(config, SELECTMAP_CONFIG_REG);

	return assert_write;
}

int fpga_spartan6_cs_fn(Xilinx_desc *desc, int assert_cs, int flush, int cookie)
{
	int config;
	config = readw(SELECTMAP_CONFIG_REG);
	if (assert_cs)
		config &= ~(1 << SELECTMAP_CONFIG_CSI_N);
	else
		config |= (1 << SELECTMAP_CONFIG_CSI_N);
	writew(config, SELECTMAP_CONFIG_REG);

	return assert_cs;
}

/* Initialize FPGA before download */
int fpga_spartan6_pre_fn(Xilinx_desc *desc, int cookie)
{
	int config;

	config = readw(SELECTMAP_CONFIG_REG);
	config &= ~(1 << SELECTMAP_CONFIG_CLK);	/* IP in selectmap mode */
	config |= (1 << SELECTMAP_CONFIG_OS);	/* OS High */
	config |= (1 << SELECTMAP_CONFIG_CSI_N);	/* CSI_n High */
	config |= (1 << SELECTMAP_CONFIG_PROGRAM_N);	/* PROGRAM_n High */
	config |= (1 << SELECTMAP_CONFIG_RDWR_N);	/* rdwr_n High */

	writew(config, SELECTMAP_CONFIG_REG);
	udelay(1);		/*wait until supply started */
	config = readw(SELECTMAP_CONFIG_REG);

	return cookie;
}

/* Set the FPGA's active-low program line to the specified level */
int fpga_spartan6_pgm_fn(Xilinx_desc *desc, int assert, int flush, int cookie)
{
	unsigned short config;

	config = readw(SELECTMAP_CONFIG_REG);

	if (assert)
		config &= ~(1 << SELECTMAP_CONFIG_PROGRAM_N);
	else
		config |= (1 << SELECTMAP_CONFIG_PROGRAM_N);

	writew(config, SELECTMAP_CONFIG_REG);

	return assert;
}
