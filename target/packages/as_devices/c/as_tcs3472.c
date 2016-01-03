/*
 * This software is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Julien Boibessot <julien.boibessot@armadeus.com>
 * Copyright (C) 2014 The Armadeus Project & Armadeus Systems
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "as_tcs3472.h"

/* registers */
#define TCS3472_REG_ENABLE	(0x00)
#define TCS3472_REG_ATIME	(0x01)
#define TCS3472_REG_WTIME	(0x02)
/* TBDL
#define TCS3472_REG_AILTL	(0x04)
#define TCS3472_REG_AILTH	(0x05)
#define TCS3472_REG_AIHTL	(0x06)
#define TCS3472_REG_AIHTH	(0x07)
#define TCS3472_REG_PERS	(0x0c)
*/
#define TCS3472_REG_CONFIG	(0x0d)
#define TCS3472_REG_CONTROL	(0x0f)
#define TCS3472_REG_ID		(0x12)
#define TCS3472_REG_STATUS	(0x13)
#define TCS3472_REG_STATUS_AVALID	(1 << 0)
#define TCS3472_REG_CDATAL	(0x14)
#define TCS3472_REG_CDATAH	(0x15)
#define TCS3472_REG_RDATAL	(0x16)
#define TCS3472_REG_RDATAH	(0x17)
#define TCS3472_REG_GDATAL	(0x18)
#define TCS3472_REG_GDATAH	(0x19)
#define TCS3472_REG_BDATAL	(0x1a)
#define TCS3472_REG_BDATAH	(0x1b)

//#define DEBUG
#ifdef DEBUG
#define DBG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...)		/*fmt, ##__VA_ARGS__ */
#endif
#define ERROR(fmt, ...) printf("%s" fmt, __func__, ##__VA_ARGS__)

static int32_t as_tcs3472_read16(struct as_tcs3472 *tcs3472, int reg,
				 unsigned short *value, int size)
{
	int ret;
	unsigned char buf[2];

	/* Set address of register to access */
	buf[0] = 0x80 | ((unsigned char)(reg & 0x1f));
	ret = as_i2c_write(tcs3472->i2c_dev, buf, 1);
	if (ret)
		return ret;

	/* Read it */
	ret = as_i2c_read(tcs3472->i2c_dev, buf, 2);
	if (ret)
		return ret;

	*value = buf[1] << 8 | buf[0];
	DBG("read 0x%02x%02x at 0x%02x\n", buf[1], buf[0], reg);

	return 0;
}

static int32_t as_tcs3472_read8(struct as_tcs3472 *tcs3472, int reg,
				unsigned char *value)
{
	int ret;
	unsigned char buff[1];

	/* Set address of register to access */
	buff[0] = 0x80 | ((unsigned char)(reg & 0x1f));
	ret = as_i2c_write(tcs3472->i2c_dev, buff, 1);
	if (ret)
		return ret;

	/* Read it */
	ret = as_i2c_read(tcs3472->i2c_dev, buff, 1);
	if (ret)
		return ret;

	*value = *buff;
	DBG("read 0x%02x in 0x%02x\n", *value, reg);

	return 0;
}

static int32_t as_tcs3472_write8(struct as_tcs3472 *tcs3472, int reg,
				 unsigned char value)
{
	int ret;
	unsigned char buf[1];

	/* Set address of register to access */
	buf[0] = 0x80 | ((unsigned char)(reg & 0x1f));
	ret = as_i2c_write(tcs3472->i2c_dev, buf, 1);
	if (ret)
		return ret;

	/* write value */
	buf[0] = value;
	ret = as_i2c_write(tcs3472->i2c_dev, buf, 1);
	if (ret)
		return ret;

	return 0;
}

/*-----------------------------------------------------------------------------*/

struct as_tcs3472 *as_tcs3472_open(int i2c_bus, int i2c_addr)
{
	struct as_tcs3472 *tcs3472;
	struct as_i2c_device *i2c_dev;
	int ret;
	unsigned char id;

	i2c_dev = as_i2c_open(i2c_bus);
	if (i2c_dev == NULL) {
		perror("Can't open I2C bus number\n");
		return NULL;
	}

	ret = as_i2c_set_slave_addr(i2c_dev, i2c_addr);
	if (ret < 0) {
		perror("Can't set i2c address\n");
		goto close_i2c_err;
	}

	tcs3472 = (struct as_tcs3472 *)malloc(sizeof(struct as_tcs3472));
	if (!tcs3472) {
		perror("can't allocate memory for device structure\n");
		goto close_i2c_err;
	}
	tcs3472->i2c_dev = i2c_dev;

	ret = as_tcs3472_read8(tcs3472, TCS3472_REG_ID, &id);
	if (ret) {
		printf("%s: can't read ID register !\n", __func__);
		goto free_tcs3472_dev_err;
	}
	if (id == 0x44) {
		printf("found a TCS34721/5");
	} else if (id == 0x42) {
		printf("found a TCS34723/7");
	} else {
		printf("No TCS3472x found at given address !!\n");
		goto free_tcs3472_dev_err;
	}
	printf(" @ 0x%02x on I2C%d\n", i2c_addr, i2c_bus);

	/* Powerdown by default */
	as_tcs3472_write8(tcs3472, TCS3472_REG_ENABLE, 0x00);

	return tcs3472;

free_tcs3472_dev_err:
	free(tcs3472);
close_i2c_err:
	as_i2c_close(i2c_dev);
	return NULL;
}

/*-----------------------------------------------------------------------------*/

int32_t as_tcs3472_start_rgbc(struct as_tcs3472 * tcs3472, int time)
{
	struct timespec t;
	unsigned char int_time;

	/* Powerup & wait 24ms */
	as_tcs3472_write8(tcs3472, TCS3472_REG_ENABLE, 0x01);
	t.tv_sec = 0;
	t.tv_nsec = 24000000;	/* 24ms */
	nanosleep(&t, NULL);

	/* Set integration time */
	int_time = (unsigned char)(256 - (time * 10 / 24));
	as_tcs3472_write8(tcs3472, TCS3472_REG_ATIME, int_time);

	/* AEN */
	as_tcs3472_write8(tcs3472, TCS3472_REG_ENABLE, 0x03);

#ifdef DEBUG
	unsigned char buf2;
	nanosleep(&t, NULL);
	as_tcs3472_read8(tcs3472, TCS3472_REG_STATUS, &buf2);
#endif

	return 0;
}

/*-----------------------------------------------------------------------------*/

int32_t as_tcs3472_get_colours(struct as_tcs3472 * tcs3472,
			       struct as_tcs3472_colours * colours)
{
	int ret;
	unsigned short buf;
	unsigned char status;

	as_tcs3472_read8(tcs3472, TCS3472_REG_STATUS, &status);
	if (!status && TCS3472_REG_STATUS_AVALID) {
		ERROR("integration cycle not completed yet\n");
		return -1;
	}

	ret = as_tcs3472_read16(tcs3472, TCS3472_REG_CDATAL, &buf, 1);
	if (ret)
		return ret;
	colours->clear = buf;

	ret = as_tcs3472_read16(tcs3472, TCS3472_REG_RDATAL, &buf, 1);
	if (ret)
		return ret;
	colours->red = buf;

	ret = as_tcs3472_read16(tcs3472, TCS3472_REG_GDATAL, &buf, 1);
	if (ret)
		return ret;
	colours->green = buf;

	ret = as_tcs3472_read16(tcs3472, TCS3472_REG_BDATAL, &buf, 1);
	if (ret)
		return ret;
	colours->blue = buf;

	return 0;
}

/*-----------------------------------------------------------------------------*/

int32_t as_tcs3472_close(struct as_tcs3472 * tcs3472)
{
	as_i2c_close(tcs3472->i2c_dev);
	free(tcs3472);

	return 0;
}
