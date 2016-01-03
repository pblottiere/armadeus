/*
 * Driver for the i.MX PWM driver
 *
 * This driver is part of the Armadeus Project and is the merge of a lot of ideas
 * found on the Web.
 *
 * Maintainer: Julien Boibessot <julien.boibessot@armadeus.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h>        /* get_user, copy_to_user */
#include <linux/miscdevice.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
#include <mach/hardware.h>
#include <linux/sysdev.h>
#endif
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/pm.h>
#include <linux/clk.h>
#include <linux/device.h>	/* struct class */
#include <linux/circ_buf.h>
#include <linux/platform_device.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,38)
#include <linux/slab.h>
#include <linux/sched.h>
#endif

#include "pwm.h"

#if defined(CONFIG_ARCH_MX2) || defined(CONFIG_SOC_IMX27) || defined(CONFIG_ARCH_MX5)
#define PWMCTRL 	(0x00)
#define PWMSTATUS 	(0x04)
#define PWMIRQ 		(0x08)
#define PWMSAMPLE 	(0x0c)
#define PWMPERIOD 	(0x10)  
#define PWMCOUNTER 	(0x14)
#define PWM_EN		(1<<0)
#define PWM_IRQEN	(1<<0)
#define PWM_SWR		(1<<3)
#define PWM_CLKSRC_MASK	(3<<16)
#if defined(CONFIG_ARCH_MX2) || defined(CONFIG_SOC_IMX27)
#define PWM_CLKSRC_IPG	(2<<16)
#else
#define PWM_CLKSRC_IPG	(1<<16)
#endif
#define PWM_CLKSRC_32K	(3<<16)
#define PWM_REPEAT_MASK	(3<<1)
#define PWM_HCTR	(1<<20)
#define PWM_BCTR	(1<<21)
#define PWM_FWM_3	(2<<26)
#define PWM_FIFOAV	(7<<0)
#define PWM_FIFO_EMPTY	(1<<3)
#define PWM_FWM		(3<<26)
#define PWM_REPEAT(x)	(((x) & 0x03) << 1)
#define PWM_CLKSEL_MASK	(0)
#define PWM_PRESCALER_MASK 	0x0000FFF0
#define PWM_PRESCALER(x)	(((x-1) & 0xFFF) << 4)
#define PWM_PERIOD(x)		((x) & 0xFFFF)
#define PWM_SAMPLE(x)		((x) & 0xFFFF)
#define PWM_INT		MXC_INT_PWM
#if defined(CONFIG_ARCH_MX2) || defined(CONFIG_SOC_IMX27)
# define PWM_MAX_DEV	1
#else
# define PWM_MAX_DEV	2
#endif

#define PWM_MIN_FREQUENCY 1		/* Hz */
#define PWM_MAX_FREQUENCY 1000000	/* Hz */

#else
#define PWMCTRL		(0x00)
#define PWMSTATUS	(0x00)
#define PWMIRQ		(0x00)
#define PWMSAMPLE	(0x04)
#define PWMPERIOD	(0x08)
#define PWMCOUNTER	(0x0C)
#define PWM_EN		(1<<4)
#define PWM_IRQEN	PWMC_IRQEN
#define PWM_SWR		PWMC_SWR
#define PWM_CLKSRC_MASK	(1<<15)
#define PWM_CLKSRC_IPG	(0<<15)
#define PWM_CLKSRC_32K	(1<<15)
#define PWM_FWM_3	(2<<26)
#define PWM_FWM		(3<<26)
#define PWM_HCTR	PWMC_HCTR
#define PWM_BCTR	PWMC_BCTR
#define PWM_FIFOAV	PWMC_FIFOAV
#define PWM_REPEAT_MASK	0x0000000C
#define PWM_CLKSEL_MASK	0x00000003
#define PWM_REPEAT(x)	PWMC_REPEAT(x)
#define PWM_PRESCALER_MASK 	0x00007F00
#define PWM_PRESCALER(x) PWMC_PRESCALER(x)
#define PWM_PERIOD(x)	PWMP_PERIOD(x)
#define PWM_SAMPLE(x)	PWMS_SAMPLE(x)
#define PWM_MAX_DEV	1

#define PWM_MIN_FREQUENCY 2       /* Hz */
#define PWM_MAX_FREQUENCY 1000000 /* Hz */

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
#define DRIVER_NAME         "imx-pwm"
#elif LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
#define DRIVER_NAME         "mxc_pwm"
#else
#define DRIVER_NAME         "imx27-pwm"
#endif

#define DRIVER_VERSION      "0.81"


struct sound_circ_buf {
	char* buf;
	int write; /* == head */
	int read;  /* == tail */
	int size;
};
#define MAX_SOUND_BUFFER_SIZE (16*1024) /* !! Should be a power of 2 !! */

typedef struct timer_list timer_blk_t;

typedef struct timerStruct
{
	timer_blk_t	*timer_blk_ptr;
	int     period;
	void    (*timer_func)(unsigned long);
	int     stop_flag;
} pwm_timer_t;

struct pwm_device {
	void __iomem *membase;
	unsigned int irq;

	int active;
	unsigned int duty;	/* current duty cycle in % x 10 */
	unsigned int frequency;	/* current frequency in Hz */
	u16 prescaler;
	u16 period_reg;	/* value to put in PWMPR register to have requested freq */

	int     mode;
	int     nbbytes; /* count 4 bytes write occurence to FIFO */
	int     dataLen;
	u16     sampleValue;

	wait_queue_head_t write_wait;
	wait_queue_head_t exit_wait;

	struct sound_circ_buf circ_buf;
	pwm_timer_t timer;
	timer_blk_t timer_blk;
	struct clk *clk;

	struct device *dev;
};

int gMajor = 0;
static struct pwm_device *dev_table[PWM_MAX_DEV];
static struct class *pwm_class;	/* forward declaration only */

static void inline unregister_sys_file(struct pwm_device *pwm);

/*
struct fasync_struct *ts_fasync;
*/

/****************************************************************
 * Circular buffer handling for sound playing
 ***************************************************************/

static inline u8 get_byte_from_circbuf(struct sound_circ_buf* abuffer)
{
	u8 b = abuffer->buf[abuffer->read];
	abuffer->read = (abuffer->read + 1) & (abuffer->size - 1);

	return b;
}

static inline u16 get_word_from_circbuf(struct sound_circ_buf* abuffer)
{
	u16 w = *((u16*)(&(abuffer->buf[ abuffer->read ])));
	abuffer->read = (abuffer->read + 2) & (abuffer->size - 1);

	return w;
}

static inline void put_byte_to_circbuf(struct sound_circ_buf* abuffer, unsigned char abyte)
{
	abuffer->buf[abuffer->write] = abyte;
	abuffer->write = (abuffer->write + 1) & (abuffer->size - 1);
}

static inline void increase_circbuf(struct sound_circ_buf* abuffer, int count)
{
	abuffer->write = (abuffer->write + count) & (abuffer->size - 1);
	pr_debug("Added %d bytes to buffer, now write is in pos %d\n", count, abuffer->write);
}


static int get_current_pwm_clk_rate(struct pwm_device *pwm)
{
	if ((readl(pwm->membase + PWMCTRL) & PWM_CLKSRC_MASK) == PWM_CLKSRC_32K)
		return 32768;
#if defined(CONFIG_ARCH_MX2) || defined(CONFIG_SOC_IMX27) || defined(CONFIG_ARCH_MX5)
	return clk_get_rate(pwm->clk);
#else
	return 8000000; 
#endif
}

/* result in Hz */
static long get_pwm_min_freq(struct pwm_device *pwm)
{
	return PWM_MIN_FREQUENCY;
}

/* result in Hz */
static long get_pwm_max_freq(struct pwm_device *pwm)
{
	return PWM_MAX_FREQUENCY;
}

/* freq in Hz */
static void compute_pwm_params(u32 req_freq, struct pwm_device *pwm)
{
	u32 input_freq, divider;

	input_freq = get_current_pwm_clk_rate(pwm);
	pr_debug("input freq, %d %d\n", input_freq, req_freq);
	divider = ((input_freq*10)/req_freq+5)/10;
	if (divider/65536) {
		pwm->prescaler = (divider/65536)+1;
		pwm->period_reg = input_freq/(pwm->prescaler*req_freq)-2;
	} else {
		pwm->period_reg = divider-2;
		pwm->prescaler = 1;
	}
	pwm->frequency = req_freq;
	pr_debug("period: %d prescaler: %d\n", pwm->period_reg, pwm->prescaler);
}


static void write_bits(u32 bit, u32 mask, void __iomem *reg)
{
	u32 temp;
	temp = readl(reg) & ~mask;
	writel(temp | (bit&mask), reg);
}

static void setup_pwm_params(struct pwm_device *pwm)
{
	write_bits(PWM_CLKSRC_IPG, PWM_CLKSRC_MASK, pwm->membase + PWMCTRL);
	/* Setup prescaler */
	write_bits(PWM_PRESCALER(pwm->prescaler),
				PWM_PRESCALER_MASK, pwm->membase + PWMCTRL); /*| PWMC_REPEAT(3)*/;
	/* Setup period */
	writel(PWM_PERIOD(pwm->period_reg), pwm->membase + PWMPERIOD);
	/* Setup duty cycle */
	writel(PWM_SAMPLE((u32)((pwm->period_reg * pwm->duty) / 1000)),
				pwm->membase + PWMSAMPLE);
}


/* common setup function whenever something was changed */
static void setup_pwm_unit(struct pwm_device *pwm)
{
	if (pwm->active) {
		/* Activate PWM */
		write_bits(PWM_EN, PWM_EN, pwm->membase + PWMCTRL);
		setup_pwm_params(pwm);
	} else {
		/* De-activate */
		write_bits(~PWM_EN, PWM_EN, pwm->membase + PWMCTRL);
	}
}

/* timer */
static int create_timer(pwm_timer_t *timer)
{
	init_timer(timer->timer_blk_ptr);
	timer->timer_blk_ptr->function = timer->timer_func;

	return 0;
}

#if 0
static int start_timer(pwm_timer_t *timer)
{
	timer->timer_blk_ptr->expires = jiffies + timer->period;
	timer->stop_flag = 0;

	add_timer((struct timer_list *)timer->timer_blk_ptr);

	return 0;
}
#endif

static int stop_timer(pwm_timer_t *timer)
{
	timer->stop_flag = 1;
	del_timer_sync((struct timer_list *)timer->timer_blk_ptr);

	return 0;
}

static void stop_pwm(struct pwm_device *pwm)
{
	write_bits(~PWM_IRQEN, PWM_IRQEN, pwm->membase + PWMIRQ);
	write_bits(~PWM_EN, PWM_EN, pwm->membase + PWMCTRL);

	pwm->sampleValue = 0;
	if (pwm->mode == PWM_TONE_MODE)
		stop_timer(&pwm->timer);

	/* Can release file now */
	wake_up_interruptible(&pwm->exit_wait);

	pr_debug("data completed.\n");
	pr_debug("PWMC = 0x%8x\n", readl(pwm->membase + PWMCTRL));
}

static void pwmIntFunc(unsigned long unused)
{
//    u32	period;

	pr_debug("pwm sam int\n");

//     if (gWriteCnt > 0)
//     {
//         PK("<1>PWMS = 0x%4x\n", (u16)_reg_PWM_PWMS);
// 
//         // 96M/128/2/period
//         period = *writeBufPtr16++;
// 
//         if(period > 0)
// //			PWMP = 96000000/128/12/2/period; //96000000/128/2*11
//             PWMP = 66000000/128/2/period; //96000000/128/2*11
//         PK("<1>PWMP = 0x%4x\n", (u16)PWMP);
//         PK("<1>PWMCNT = 0x%4x\n", (u16)PWMCNT);
// 
//         StartTimer(&timer);
// 
//         gWriteCnt--;
//     }
//     else
//         StopPwm();

}

static int init_pwm(struct pwm_device *pwm)
{
	/* Software reset */
	write_bits(PWM_SWR, PWM_SWR, pwm->membase + PWMCTRL);
	udelay(10);
	/* Activate & de-activate PWM (seems to be necessary after a reset) */
	write_bits(PWM_EN, PWM_EN, pwm->membase + PWMCTRL);
	write_bits(~PWM_EN, PWM_EN, pwm->membase + PWMCTRL);

	return 1;
}


/* /DEV INTERFACE */

int pwm_release(struct inode * inode, struct file * filp)
{
	struct pwm_device *pwm = dev_table[iminor(inode)];

	/* wait unit gWriteCnt == 0 */
	interruptible_sleep_on(&pwm->exit_wait);
	if (pwm->mode == PWM_PLAY_MODE) {
		/* disable IRQ */
		write_bits(~PWM_IRQEN, PWM_IRQEN, pwm->membase + PWMIRQ);
	}	
	write_bits(~PWM_EN, PWM_EN, pwm->membase + PWMCTRL);

	if (pwm->circ_buf.buf) {
		kfree(pwm->circ_buf.buf);
		pwm->circ_buf.buf = 0;
	}
	printk(DRIVER_NAME " released, 4 bytes FIFO usage: %d\n", pwm->nbbytes);

	return 0;
}

int pwm_open(struct inode * inode, struct file * filp)
{
	struct pwm_device *pwm = dev_table[iminor(inode)];

	pwm->circ_buf.buf = 0;
	pwm->dataLen = PWM_DATA_8BIT;
	pwm->nbbytes = 0;
	/* Init PWM hardware */
	init_pwm(pwm);
	
	pr_debug(DRIVER_NAME " opened \n");

	return 0;
}

static int pwm_fasync(int fd, struct file *filp, int mode)
{
	pr_debug("in pwm_fasyn ----\n");
#if 0
    /* TODO TODO put this data into file private data */
    int minor = checkDevice( filp->f_dentry->d_inode);
    if ( minor == - 1)
    {
        PK("<1>asp_fasyn:bad device minor\n");
        return -ENODEV;
    }

    return( fasync_helper(fd, filp, mode, &ts_fasync) );
#endif
	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
int pwm_ioctl(struct inode * inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,7,0)
long pwm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#else
int pwm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#endif
#endif
{
	char *str=NULL;
	int ret = 0;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	struct pwm_device *pwm = dev_table[iminor(inode)];
#else
	struct pwm_device *pwm = dev_table[iminor(filp->f_dentry->d_inode)];
#endif
	switch (cmd) {
		/* Set PWM Mode (Tone or Playback) */
		case PWM_IOC_SMODE:
		{
			pwm->mode = arg;
	
			if (pwm->mode == PWM_TONE_MODE) {
				/* create periodic timer when tone mode */
				pr_debug("PWM Tone Mode.\n");
				pwm->timer.timer_blk_ptr = &pwm->timer_blk;
				pwm->timer.timer_func = pwmIntFunc;
				create_timer(&pwm->timer);
			} else {
				pr_debug("PWM Play Mode.\n");
			}
		}
		break;

		/* Set Playback frequency/ouput rate */
		case PWM_IOC_SFREQ:
		{
			/*int clock = get_current_pwm_clk_rate(pwm);*/
			/* Disable PWM */
			write_bits(~PWM_EN, PWM_EN, pwm->membase + PWMCTRL);

/*			if(!( (clock >= 16000000)  && (clock<17000000))){
				printk("audio playback works only with a 16MHz input clock %d!\n", 
						get_current_pwm_clk_rate(pwm));
				break;
			}*/
			write_bits(PWM_CLKSRC_IPG, PWM_CLKSRC_MASK, pwm->membase + PWMCTRL);
#if defined(CONFIG_ARCH_MX2) || defined(CONFIG_SOC_IMX27) || defined(CONFIG_ARCH_MX5)
			write_bits(PWM_PRESCALER(2), PWM_PRESCALER_MASK, pwm->membase + PWMCTRL);
#else
			write_bits(PWM_PRESCALER(1), PWM_PRESCALER_MASK, pwm->membase + PWMCTRL);
#endif
			switch (arg) {
				case PWM_SAMPLING_32KHZ:
					/* REPEAT = 0 => divide by 1 */
					write_bits(0, PWM_REPEAT_MASK, pwm->membase + PWMCTRL);
					str = "32Khz";
				break;

				case PWM_SAMPLING_16KHZ:
					/* REPEAT = 01 => divide by 2 */
					write_bits(PWM_REPEAT(1), PWM_REPEAT_MASK, pwm->membase + PWMCTRL);
					str = "16Khz";
				break;
	
				case PWM_SAMPLING_8KHZ:
				default:
					/* REPEAT = 10 => divide by 4*/
					write_bits(PWM_REPEAT(2), PWM_REPEAT_MASK, pwm->membase + PWMCTRL);
					str = "8Khz";
				break;
			}
			pr_debug("Sample rate = %s\n", str);
			pr_debug("ioctl: PWMC = 0x%8x\n", readl(pwm->membase+PWMCTRL));
		}
		break;

		/* Set samples length (8 or 16 bits) */
		case PWM_IOC_SDATALEN:
		{
			pwm->dataLen = arg;
			if (arg == PWM_DATA_8BIT) {
				writel(0xfe, pwm->membase + PWMPERIOD);
				str = "8bit";
			} else { /* if(arg == PWM_DATA_16BIT) */
				writel(0xfffe, pwm->membase + PWMPERIOD);
				str = "16bit";
			}
			pr_debug("Data Length = %s\n", str);
		}
		break;

		case PWM_IOC_SSAMPLE:
			pwm->sampleValue = arg;
		break;

		case PWM_IOC_SPERIOD:
			if (pwm->mode == PWM_TONE_MODE) {
				pr_debug("PWM period = %d\n", (int)arg);
				pwm->timer.period = arg/12;
			}
		break;

		case PWM_IOC_STOP:
			stop_pwm(pwm);
		break;

		case PWM_IOC_SWAPDATA:
			if (arg & PWM_SWAP_HCTRL) { /* Halfword FIFO data swapping */
				write_bits(PWM_REPEAT(1), PWM_REPEAT_MASK, pwm->membase + PWMCTRL);
				write_bits(PWM_HCTR, PWM_HCTR, pwm->membase + PWMCTRL);
			} else {
				write_bits(~PWM_HCTR, PWM_HCTR, pwm->membase + PWMCTRL);
			}
		
			if (arg & PWM_SWAP_BCTRL) { /* Byte FIFO data swapping */
				write_bits(PWM_BCTR, PWM_BCTR, pwm->membase + PWMCTRL);
			} else {
				write_bits(~PWM_BCTR, PWM_BCTR, pwm->membase + PWMCTRL);
			}
		break;

		default:
			printk(DRIVER_NAME ": unkown IOCTL\n");
		break;
	}

	return ret;
}

ssize_t pwm_read(struct file * filp, char * buf, size_t count, loff_t * l)
{
	int ret = 0;
	struct pwm_device *pwm = dev_table[iminor(filp->f_dentry->d_inode)];

	if (readl(pwm->membase+PWMIRQ) & PWM_IRQEN)
		ret = 1; /* no data */
	else
		ret = 0; /* processing data */

	return ret;
}

static ssize_t pwm_write( struct file *filp, const char *buf, size_t count, loff_t *f_pos )
{
	int ret = 0;
	int remaining_space = 0;
	struct pwm_device *pwm = dev_table[iminor(filp->f_dentry->d_inode)];
	struct sound_circ_buf *circ = &pwm->circ_buf;

	if (count > MAX_SOUND_BUFFER_SIZE)
		count = MAX_SOUND_BUFFER_SIZE;
	/* First time, allocate the double buffer */
	if (!circ->buf) {
		circ->buf = kmalloc(MAX_SOUND_BUFFER_SIZE*2, GFP_KERNEL);
		if (!circ->buf) {
			goto out;
		} else {
			/* Initialize circular buffer */
			circ->read  = 0;
			circ->write = 0;
			circ->size  = MAX_SOUND_BUFFER_SIZE*2;
		}
	}
	pr_debug("write %x, %x, %x\n", readl(pwm->membase + PWMCTRL), readl(pwm->membase + PWMIRQ), readl(pwm->membase + PWMSTATUS));

	/* If there is enough space at the end of the buffer for all data in one copy, do it */
	remaining_space = circ->size - circ->write;
	pr_debug("Remaining space to end: %d, must write: %d \n", remaining_space, count);
	if (remaining_space >= count) {
		// Get data from UserSpace
		if (copy_from_user(circ->buf + circ->write, buf, count)) {
			ret = -EFAULT;
			goto out;
		}
		increase_circbuf(&pwm->circ_buf, count);
	/* if not enough space, then */
	} else {
		/* Do it in 2 times */
		if (copy_from_user(circ->buf + circ->write, buf, remaining_space)) {
			ret = -EFAULT; goto out;
		}
		increase_circbuf(&pwm->circ_buf, remaining_space);
		if (copy_from_user(circ->buf + circ->write, buf, count - remaining_space)) {
			ret = -EFAULT; goto out;
		}
		increase_circbuf(&pwm->circ_buf, count - remaining_space);
	}

	if (pwm->mode == PWM_PLAY_MODE) {
		/* Enable IRQ */
		write_bits(PWM_IRQEN, PWM_IRQEN, pwm->membase + PWMIRQ);
	} else {
		writel(pwm->sampleValue, pwm->membase + PWMSAMPLE);
		/* set prescaler to the max */
		write_bits(PWM_PRESCALER(0xffff), PWM_PRESCALER_MASK, pwm->membase + PWMCTRL);
		/*writeBufPtr16 = (u16*)gpWriteBuf;
		gWriteCnt /= 2; //input size is 8bit size, need /2 to get 16bit size
		StartTimer( &timer );*/
	}
	writel(0x00000000, pwm->membase + PWMSAMPLE);
	write_bits(PWM_FWM_3, PWM_FWM, pwm->membase + PWMCTRL);
	/* Enable PWM */
	write_bits(PWM_EN, PWM_EN, pwm->membase + PWMCTRL);

	/* Calculate how much space is left in buffer */
	remaining_space = CIRC_SPACE(circ->write, circ->read, circ->size);
	pr_debug("Space left: %d\n", remaining_space);
	if (remaining_space < count) {
		/* Pauses user app until interrupt handler has consumed some data */
		interruptible_sleep_on(&pwm->write_wait);
	}

out:
	pr_debug("out pwm_write PWMC = 0x%8x ----\n", readl(pwm->membase + PWMCTRL));
	return count;
}

struct file_operations pwm_fops = {
	open:           pwm_open,
	release:        pwm_release,
	read:           pwm_read,
	write:          pwm_write,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	ioctl:		pwm_ioctl,
#else
	unlocked_ioctl:	pwm_ioctl,
#endif
	fasync:         pwm_fasync
};

/* END OF /DEV INTERFACE */


/* /sys interface: */

static ssize_t pwm_show_duty(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret_size = 0;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	ret_size = sprintf(buf, "%u", pwm->duty);

	return ret_size;
}

/*
 * Duty value can be 0 ... 1000. Without floating point you can take
 * one position after decimal point into account. The internal counters
 * supports 10 bit resolution, so it makes sense to support such a thing
 * like 66.6% or 33.3% (given as 666 and 333 values)
 */
static ssize_t pwm_store_duty(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	long value;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	value = simple_strtol(buf, NULL, 10);
	if ((value < 1) || (value > 999))
		return -EIO;

	pwm->duty = (unsigned)value;
	setup_pwm_unit(pwm);

	return size;
}

static ssize_t pwm_show_period(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret_size = 0;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	if (pwm->frequency)
		ret_size = sprintf(buf, "%d", (u32) 1000000/(pwm->frequency));
	else
		ret_size = sprintf(buf, "0");

	return ret_size;
}

static ssize_t pwm_store_period(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	long value;
	struct pwm_device *pwm = dev_get_drvdata(dev);
	
	value = simple_strtol(buf, NULL, 10); /* in us */
	if ((value < (1000000/get_pwm_max_freq(pwm))) || (value > (1000000/get_pwm_min_freq(pwm))))
		return -EIO;

	compute_pwm_params(1000000/value, pwm);
	setup_pwm_unit(pwm);

	return size;
}

static ssize_t pwm_show_frequency(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret_size = 0;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	ret_size = sprintf(buf, "%d", pwm->frequency);

	return ret_size;
}

/*
 * Given value (in Hertz) should be between 2 and 100k
 */

static ssize_t pwm_store_frequency(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	long value;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	value = simple_strtol(buf, NULL, 10);
	if ((value < get_pwm_min_freq(pwm)) || (value > get_pwm_max_freq(pwm)))
		return -EIO;

	compute_pwm_params(value, pwm);
	setup_pwm_unit(pwm);

	return size;
}

static ssize_t pwm_show_state(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret_size = 0;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	ret_size = sprintf(buf, "%d", pwm->active);

	return ret_size;
}

static ssize_t pwm_store_state(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	long value;
	struct pwm_device *pwm = dev_get_drvdata(dev);

	value = simple_strtol(buf, NULL, 10);
	if (value != 0)
		pwm->active = 1;
	else
		pwm->active = 0;

	setup_pwm_unit(pwm);

	return size;
}

/* dev_attr_duty  /class/pwm/pwmX/duty */
static DEVICE_ATTR(duty, S_IWUSR | S_IRUGO, pwm_show_duty, pwm_store_duty);
/* dev_attr_period  /sys/class/pwm/pwmX/period */
static DEVICE_ATTR(period, S_IWUSR | S_IRUGO, pwm_show_period, pwm_store_period);
/* dev_attr_frequency  /sys/class/pwm/pwmX/frequency */
static DEVICE_ATTR(frequency, S_IWUSR | S_IRUGO, pwm_show_frequency, pwm_store_frequency);
/* dev_attr_active  /sys/class/pwm/pwmX/active */
static DEVICE_ATTR(active, S_IWUSR | S_IRUGO, pwm_show_state, pwm_store_state);


/*
 *  PWM interrupt handler (used in Play Mode only)
 */
static irqreturn_t pwm_interrupt(int irq, void *dev_id)
{
#if !defined(CONFIG_ARCH_MX2) && !defined(CONFIG_SOC_IMX27) && !defined(CONFIG_ARCH_MX5)
	u32 status = 0;
#endif
	int remaining = 0;
	struct pwm_device *pwm = (struct pwm_device *) dev_id;

	/* Acknowledge interrupt */
#if defined(CONFIG_ARCH_MX2) || defined(CONFIG_SOC_IMX27) || defined(CONFIG_ARCH_MX5)
	write_bits(PWM_FIFO_EMPTY, PWM_FIFO_EMPTY, pwm->membase + PWMSTATUS);
#else
	status = readl(pwm->membase + PWMSTATUS);
#endif
	remaining = CIRC_CNT(pwm->circ_buf.write, pwm->circ_buf.read, pwm->circ_buf.size);
	if (remaining <= 1024 && remaining > 1021) { /* TODO make it better (I think a test each consumed sample is the only accurate solution */
		/* Can accept new data now */
		wake_up_interruptible(&pwm->write_wait);
	}

	/* End of sound buffer */
	if (remaining < 3) {
		if (pwm->dataLen == PWM_DATA_8BIT) {
			while (remaining) {
				writel((u32)(get_byte_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
				/*pr_debug("(%d) pwm_int: PWMS = 0x%8x\n", gWriteCnt, PWMS);*/
				remaining--;
			}
		} else { /* PWM_DATA_16BIT) */
			while (remaining) {
				writel((u32)(get_word_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
				/*pr_debug("(%d) pwm_int: PWMS = 0x%8x\n", gWriteCnt, PWMS); */
				remaining--;
			}
		}
		stop_pwm(pwm);
		goto end;
	}

	/* Put sound samples in FIFO */
	if (pwm->dataLen == PWM_DATA_8BIT) {
		writel((u32)(get_byte_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
		writel((u32)(get_byte_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
		writel((u32)(get_byte_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
#if defined(CONFIG_ARCH_MX2) || defined(CONFIG_SOC_IMX27) || defined(CONFIG_ARCH_MX5)
		if ((readl(pwm->membase + PWMSTATUS) & PWM_FIFOAV) == 3) { /* TODO: verify if needed */
			writel((u32)(get_byte_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
		}
#else
		if ((readl(pwm->membase + PWMSTATUS) & PWM_FIFOAV) > 0) { /* TODO: verify if needed */
			writel((u32)(get_byte_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
		}
#endif
	} else { /* PWM_DATA_16BIT) */
		/* TODO: put good value in PWMC_BCTR to auto swap bytes if needed (do it at write or ioctl) */
		writel((u32)(get_word_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
		writel((u32)(get_word_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
		writel((u32)(get_word_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
		if ((readl(pwm->membase + PWMSTATUS) & PWM_FIFOAV) > 0) { /* TODO: verify if needed */
			writel((u32)(get_word_from_circbuf(&pwm->circ_buf)), pwm->membase + PWMSAMPLE);
			pwm->nbbytes++;
		}
	}

end:
	return IRQ_HANDLED;
}

static int imx_pwm_drv_probe(struct platform_device *pdev)
{
	int err = -ENODEV;
	struct resource *res;
	struct pwm_device *pwm;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	struct imx_pwm_platform_data *pdata = pdev->dev.platform_data;
#endif

	if (pdev->id >= PWM_MAX_DEV)
		dev_err(&pdev->dev, "failed. Unknown module. Remember that this device only supports %d PWM\n", PWM_MAX_DEV);

	if (dev_table[pdev->id] != NULL) {
		dev_err(&pdev->dev, "already in use\n");
		return -ENODEV;
	}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	if (!pdata) {
		dev_err(&pdev->dev, "platform data not supplied\n");
		return -ENOENT;
	}
#endif

	pwm = kmalloc(sizeof(struct pwm_device), GFP_KERNEL);
	if (!pwm) {
		return -ENOMEM;
	}
	pwm->active = 0;
	pwm->duty = 500; /* = 50.O% */
	pwm->frequency = 0;
	platform_set_drvdata(pdev, pwm);
	dev_table[pdev->id] = pwm;

	pwm->irq = platform_get_irq(pdev, 0);
	if (pwm->irq < 0) {
		dev_err(&pdev->dev, "no interrupt defined\n");
		err = -ENOENT;
		goto error_malloc;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "unable to get mem ressource\n");
		err = -ENOENT;
		goto error_malloc;
	}

	if (unlikely(!request_mem_region(res->start, res->end - res->start + 1, DRIVER_NAME))) {
		dev_err(&pdev->dev, "mem region busy\n");
		err = -EBUSY;
		goto error_malloc;
	}
	pwm->membase = ioremap(res->start, resource_size(res));
        if (!pwm->membase) {
                err = -ENOMEM;
                goto error_memreg;
        }

#ifndef CONFIG_ARCH_MX5
	/* Register our char device */
	err = register_chrdev(gMajor, DRIVER_NAME, &pwm_fops);
	if (err < 0) {
		dev_err(&pdev->dev, "unable to register char driver\n");
		goto error_iounmap;
	}
	/* Dynamic Major allocation */
	if (gMajor == 0) {
		gMajor = err;
		dev_dbg(&pdev->dev, "major = %d\n", gMajor);
	}
#endif
	err = request_irq(pwm->irq, pwm_interrupt, IRQF_DISABLED, DRIVER_NAME, pwm);
	if (err) {
		dev_err(&pdev->dev, "can't reserve irq=%d\n", pwm->irq);
		goto error_chrdev;
	}

	/* init wait queue */
	init_waitqueue_head(&pwm->write_wait);
	init_waitqueue_head(&pwm->exit_wait);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	/* init gpio */
	if (pdata->init)
		pdata->init();
#endif

	/* Create /dev */
	pwm->dev = device_create(pwm_class, NULL, MKDEV(gMajor, pdev->id), NULL, "pwm%i", pdev->id);
	if (IS_ERR(pwm->dev)) {
		err = PTR_ERR(pwm->dev);
		dev_err(&pdev->dev, "can't create device\n");
		goto error_platform_init;
        }
	dev_set_drvdata(pwm->dev, pwm);

	/* Register the attributes */
	err |= device_create_file(pwm->dev, &dev_attr_duty);
	if (unlikely(err))
		goto error_dev_create;
	err |= device_create_file(pwm->dev, &dev_attr_period);
	if (unlikely(err)) {
		dev_err(&pdev->dev, "can't create /sys/.../period\n");
		goto error_file;
	}
	err |= device_create_file(pwm->dev, &dev_attr_frequency);
	if (unlikely(err)) {
		dev_err(&pdev->dev, "can't create /sys/.../frequency\n");
		goto error_file;
	}
	err |= device_create_file(pwm->dev, &dev_attr_active);
	if (unlikely(err)) {
		dev_err(&pdev->dev, "can't create /sys/.../active\n");
		goto error_file;
	}

#if defined(CONFIG_ARCH_MX2) || defined(CONFIG_SOC_IMX27) || defined(CONFIG_ARCH_MX5)
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	pwm->clk = clk_get(&pdev->dev, "pwm_clk");
#elif LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
	pwm->clk = clk_get(&pdev->dev, "pwm");
#else
	pwm->clk = clk_get(&pdev->dev, "per");
#endif
	if (IS_ERR(pwm->clk)) {
		dev_err(&pdev->dev, "can't get clock\n");
		goto error_file;
	}	
	clk_enable(pwm->clk);
#endif
	/* init PWM hardware module */
	init_pwm(pwm);

	dev_info(&pdev->dev, "initialized\n");
	return 0;

error_file:
	unregister_sys_file(pwm);
error_dev_create:
	device_destroy(pwm_class, MKDEV(gMajor, pdev->id));
error_platform_init:
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	if (pdata->exit)
		pdata->exit();
#endif
	free_irq(pwm->irq, pwm);
error_chrdev:
	unregister_chrdev(gMajor, DRIVER_NAME);
#ifndef CONFIG_ARCH_MX5
error_iounmap:
#endif
        iounmap(pwm->membase);
error_memreg:
	release_mem_region(res->start, resource_size(res));
error_malloc:
	kfree(pwm);

	return err;
}

static inline void unregister_sys_file(struct pwm_device *pwm)
{
	/* Unregister /sys attributes */
	device_remove_file(pwm->dev, &dev_attr_active);
	device_remove_file(pwm->dev, &dev_attr_period);
	device_remove_file(pwm->dev, &dev_attr_frequency);
	device_remove_file(pwm->dev, &dev_attr_duty);
}

static int imx_pwm_drv_remove(struct platform_device *pdev)
{
	struct resource *res;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	struct pwm_device *pwm = (struct pwm_device*)pdev->dev.driver_data;
	struct imx_pwm_platform_data *pdata = pdev->dev.platform_data;
#else
	struct pwm_device *pwm = (struct pwm_device*)(dev_get_drvdata(&pdev->dev));
#endif

	unregister_sys_file(pwm);
	device_destroy(pwm_class, MKDEV(gMajor, pdev->id));
	free_irq(pwm->irq, pwm);
#ifndef CONFIG_ARCH_MX5
	unregister_chrdev(gMajor, DRIVER_NAME);
#endif
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, res->end - res->start + 1);
	kfree(pwm);
	dev_table[pdev->id] = NULL;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	if (pdata->exit())
		pdata->exit();
#endif

	return 0;
}

#ifdef CONFIG_PM
static int imx_pwm_drv_suspend(struct platform_device *pdev, pm_message_t state)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	struct pwm_device *pwm = (struct pwm_device*)pdev->dev.driver_data;
	struct imx_pwm_platform_data *pdata = pdev->dev.platform_data;
#else
	struct pwm_device *pwm = (struct pwm_device*)(dev_get_drvdata(&pdev->dev));
#endif

	clk_disable(pwm->clk);
	dev_dbg(&pdev->dev, "suspended\n");
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	if (pdata->exit())
		pdata->exit();
#endif

	return 0;
}

static int imx_pwm_drv_resume(struct platform_device *pdev)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	struct pwm_device *pwm = (struct pwm_device*)pdev->dev.driver_data;
	struct imx_pwm_platform_data *pdata = pdev->dev.platform_data;
#else
	struct pwm_device *pwm = (struct pwm_device*)(dev_get_drvdata(&pdev->dev));
#endif

	clk_enable(pwm->clk);
	dev_dbg(&pdev->dev, "resumed\n");
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	if (pdata->init())
		pdata->init();
#endif

	return 0;
}
#else

# define imx_pwm_drv_suspend NULL
# define imx_pwm_drv_resume NULL

#endif /* CONFIG_PM */

static struct platform_driver imx_pwm_driver = {
	.probe      = imx_pwm_drv_probe,
	.remove     = imx_pwm_drv_remove,
	.suspend    = imx_pwm_drv_suspend,
	.resume     = imx_pwm_drv_resume,
	.driver     = {
		.name   = DRIVER_NAME,
	},
};

static int __init imx_pwm_init(void)
{
	printk("i.MX PWM driver v" DRIVER_VERSION "\n");

	pwm_class = class_create(THIS_MODULE, "pwm");
	if (IS_ERR(pwm_class)) {
		return PTR_ERR(pwm_class);
	}

	return platform_driver_register(&imx_pwm_driver);
}

static void __exit imx_pwm_exit(void)
{
	int i;

	platform_driver_unregister(&imx_pwm_driver);
	class_destroy(pwm_class);
	for (i=0; i<PWM_MAX_DEV; i++)
		dev_table[i] = NULL;

	printk(DRIVER_NAME " successfully unloaded\n");
}

module_init(imx_pwm_init);
module_exit(imx_pwm_exit);
MODULE_AUTHOR("Julien Boibessot & Sebastien Royen, inspired by a lot of other PWM drivers");
MODULE_DESCRIPTION("i.MX PWM driver");
MODULE_LICENSE("GPL");

