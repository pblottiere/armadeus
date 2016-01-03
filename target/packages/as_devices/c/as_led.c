/*
 **    C wrapper for LEDs usage.
 **
 **    Copyright (C) 2013  The Armadeus Project - Armadeus Systems
 **    Sébastien Royen <sebastien.royen@armadeus.com>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2.1 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>		/* for close() */
#include <errno.h>
#include <limits.h>

#include "as_helpers.h"
#include "as_led.h"

#undef ERROR
#define ERROR(fmt, ...) printf(fmt, ##__VA_ARGS__)

#define SYS_CLASS_LEDS_PATH "/sys/class/leds"

/*------------------------------------------------------------------------------*/

static int32_t led_echo(struct as_led_device *a_led_dev, char *a_filename,
			char *a_content, int32_t a_length)
{
	char buf[PATH_MAX];
	int32_t result = -1;
	int fd;
	char *fake_root = as_helpers_get_root();

	snprintf(buf, sizeof(buf), "%s" SYS_CLASS_LEDS_PATH "/%s/%s",
		 fake_root, a_led_dev->led_name, a_filename);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		ERROR("Can't open %s\n", buf);
	} else {
		if (a_length != write(fd, a_content, a_length)) {
			ERROR("Error when writing %s to %s\n", a_content, buf);
		} else {
			result = 0;
		}
		close(fd);
	}
	return result;
}

/*------------------------------------------------------------------------------*/

struct as_led_device *as_led_open(char *a_led_name)
{
	struct as_led_device *dev;
	char buf[PATH_MAX];
	char *fake_root = as_helpers_get_root();

	dev = malloc(sizeof(struct as_led_device));
	if (dev == NULL) {
		ERROR("Can't allocate as_led_device structure\n");
	} else {
		snprintf(dev->led_name, sizeof(dev->led_name), "%s",
			 a_led_name);
		snprintf(buf, sizeof(buf),
			 "%s" SYS_CLASS_LEDS_PATH "/%s/brightness", fake_root,
			 dev->led_name);
		dev->file_brightness = open(buf, O_RDWR);
		if (dev->file_brightness < 0) {
			ERROR("Can't open %s\n", buf);
		} else {
			snprintf(buf, sizeof(buf),
				 "%s" SYS_CLASS_LEDS_PATH "/%s/max_brightness",
				 fake_root, dev->led_name);
			dev->file_max_brightness = open(buf, O_RDONLY);
			if (dev->file_max_brightness < 0) {
				ERROR("Can't open %s\n", buf);
			} else {
				return dev;
			}
			close(dev->file_brightness);
		}
		free(dev);
		dev = NULL;
	}

	return NULL;
}

/*------------------------------------------------------------------------------*/

int32_t as_led_close(struct as_led_device * a_led_dev)
{
	if (a_led_dev == NULL) {
		ERROR("device is NULL\n");
		return -1;
	}
	close(a_led_dev->file_max_brightness);
	close(a_led_dev->file_brightness);
	free(a_led_dev);
	return 0;
}

/*------------------------------------------------------------------------------*/

int32_t as_led_get_max_brightness(struct as_led_device * a_led_dev)
{
	int32_t result;

	if (0 == as_read_int(a_led_dev->file_max_brightness, &result)) {
		return result;
	}
	return -1;
}

/*------------------------------------------------------------------------------*/

int32_t as_led_set_brightness(struct as_led_device * a_led_dev,
			      uint32_t a_brightness)
{
	return as_write_buffer(a_led_dev->file_brightness, a_brightness);
}

/*------------------------------------------------------------------------------*/

int32_t as_led_get_brightness(struct as_led_device * a_led_dev)
{
	int32_t result;

	if (0 == as_read_int(a_led_dev->file_brightness, &result)) {
		return result;
	}
	return -1;
}

/*------------------------------------------------------------------------------*/

int32_t as_led_start_blinking(struct as_led_device * a_led_dev,
			      int32_t a_delay_on, int32_t a_delay_off)
{
	char buf[64];
	int32_t length;
	int32_t result = -1;

	if (led_echo(a_led_dev, "trigger", "timer\n", 6) == 0) {
		length = snprintf(buf, sizeof(buf), "%d\n", a_delay_on);
		if (length > 0
		    && led_echo(a_led_dev, "delay_on", buf, length) == 0) {
			length =
			    snprintf(buf, sizeof(buf), "%d\n", a_delay_off);
			if (length > 0
			    && led_echo(a_led_dev, "delay_off", buf,
					length) == 0) {
				result = 0;
			}
		}
	}
	return result;
}

/*------------------------------------------------------------------------------*/

int32_t as_led_stop_blinking(struct as_led_device * a_led_dev)
{
	if (led_echo(a_led_dev, "trigger", "none\n", 5) == 0) {
		return 0;
	}
	return -1;
}
