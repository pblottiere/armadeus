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
 *
 * Copyright (C) 2013 Sébastien Royen <sebastien.royen@armadeus.com>
 *
 *
 * TODO:
 *       - use type in /sys/bus/iio/devices/iio:device:X/scan_elements to format result
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "as_adc_iio.h"
#include "as_helpers.h"

#undef ERROR
#define ERROR(fmt, ...) printf(fmt, ##__VA_ARGS__)

#define IIO_DEVICE_MAX_CHANNELS 32

struct as_adc_iio_device {
	int channelDesc[IIO_DEVICE_MAX_CHANNELS];
};

static int open_channels(struct as_adc_iio_device *aDev, int aDeviceNum)
{
	char buffer[128];
	int i;
	int count;

	count = 0;
	for (i = 0; i < IIO_DEVICE_MAX_CHANNELS; i++) {
		snprintf(buffer, sizeof(buffer),
			 "/sys/bus/iio/devices/iio:device%d/in_voltage%d_raw",
			 aDeviceNum, i);
		aDev->channelDesc[i] = open(buffer, O_RDONLY);
		if (aDev->channelDesc[i] >= 0) {
			count++;
		}
	}
	return count;
}

static void close_channels(struct as_adc_iio_device *aDev)
{
	int i;

	for (i = 0; i < IIO_DEVICE_MAX_CHANNELS; i++) {
		if (aDev->channelDesc[i] >= 0) {
			close(aDev->channelDesc[i]);
		}
	}
}

struct as_adc_device *as_adc_open_iio(int aDeviceNum, int aVRef)
{
	struct as_adc_device *adc_dev;
	struct as_adc_iio_device *dev;

	dev = (struct as_adc_iio_device *)
	    malloc(sizeof(struct as_adc_iio_device));
	if (dev == NULL) {
		ERROR("Error during memory allocation\n");
		return NULL;
	}

	if (open_channels(dev, aDeviceNum) == 0) {
		ERROR("No in voltage channel found\n");
		goto err;
	}

	adc_dev = (struct as_adc_device *)malloc(sizeof(struct as_adc_device));
	if (dev == NULL) {
		close_channels(dev);
		ERROR("Error during memory allocation\n");
		goto err;
	}

	adc_dev->chip_param = (void *)dev;
	adc_dev->device_type = AS_IIO_NAME;
	adc_dev->device_num = aDeviceNum;
	adc_dev->vref = aVRef;	/* NOT USED */

	return adc_dev;

err:
	free(dev);
	return NULL;
}

int32_t as_adc_get_value_in_millivolts_iio(struct as_adc_device * aDev,
					   int aChannel)
{
	struct as_adc_iio_device *dev;
	int32_t value;
	int32_t ret;

	dev = (struct as_adc_iio_device *)aDev->chip_param;

	if ((aChannel >= IIO_DEVICE_MAX_CHANNELS) || (aChannel < 0)) {
		ERROR("Wrong num channel\n");
		return -1;
	}

	if (dev->channelDesc[aChannel] != -1) {
		ret = as_read_int(dev->channelDesc[aChannel], &value);
		if (ret < 0) {
			ERROR("Can't read input file\n");
			return -1;
		}
	}

	return value;
}

int32_t as_adc_close_iio(struct as_adc_device * aDev)
{
	struct as_adc_iio_device *dev =
	    (struct as_adc_iio_device *)aDev->chip_param;

	close_channels(dev);
	free(dev);
	return 0;
}
