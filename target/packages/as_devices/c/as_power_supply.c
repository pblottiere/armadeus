/*
 **    C wrapper for GPIOs usage.
 **
 **    Copyright (C) 2013  The Armadeus Project - ARMadeus Systems
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

#define DEBUG
#include "as_helpers.h"
#include "as_power_supply.h"

#define SYS_CLASS_POWER_SUPPLY_PATH "/sys/class/power_supply"

/*------------------------------------------------------------------------------*/

struct as_power_supply_device *as_power_supply_open(char *a_power_supply_name)
{
	struct as_power_supply_device *dev;
	char buf[128];
	char *fake_root;

	fake_root = as_helpers_get_root();

	dev = malloc(sizeof(struct as_power_supply_device));
	if (dev == NULL) {
		ERROR("Can't allocate as_power_supply_device structure\n");
	} else {
		snprintf(dev->power_supply_name, sizeof(dev->power_supply_name),
			 "%s", a_power_supply_name);

		snprintf(buf, sizeof(buf),
			 "%s" SYS_CLASS_POWER_SUPPLY_PATH "/%s/online",
			 fake_root, dev->power_supply_name);
		dev->file_online = open(buf, O_RDONLY);
		if (dev->file_online < 0) {
			ERROR("Can't open %s\n", buf);
		}

		snprintf(buf, sizeof(buf),
			 "%s" SYS_CLASS_POWER_SUPPLY_PATH "/%s/status",
			 fake_root, dev->power_supply_name);
		dev->file_status = open(buf, O_RDONLY);
		if (dev->file_status < 0) {
			ERROR("Can't open %s\n", buf);
		}

		snprintf(buf, sizeof(buf),
			 "%s" SYS_CLASS_POWER_SUPPLY_PATH "/%s/capacity",
			 fake_root, dev->power_supply_name);
		dev->file_capacity = open(buf, O_RDONLY);
		if (dev->file_capacity < 0) {
			ERROR("Can't open %s\n", buf);
		}
	}
	return dev;
}

/*------------------------------------------------------------------------------*/

int32_t as_power_supply_close(struct as_power_supply_device *
			      a_power_supply_dev)
{
	if (a_power_supply_dev == NULL) {
		ERROR("device is NULL\n");
		return -1;
	}
	if (a_power_supply_dev->file_online >= 0)
		close(a_power_supply_dev->file_online);
	if (a_power_supply_dev->file_status >= 0)
		close(a_power_supply_dev->file_status);
	if (a_power_supply_dev->file_capacity >= 0)
		close(a_power_supply_dev->file_capacity);
	free(a_power_supply_dev);
	return 0;
}

/*------------------------------------------------------------------------------*/

int32_t as_power_supply_is_online(struct as_power_supply_device *
				  a_power_supply_dev)
{
	int32_t value;
	int32_t ret;

	value = -1;
	if (a_power_supply_dev->file_online != -1) {
		ret = as_read_int(a_power_supply_dev->file_online, &value);
		if (ret < 0) {
			ERROR("Can't read input file\n");
			value = -1;
		}
	}

	return value;
}

/*------------------------------------------------------------------------------*/

int32_t as_power_supply_get_status(struct as_power_supply_device *
				   a_power_supply_dev, char *a_buffer,
				   uint32_t a_length)
{
	int32_t ret;

	ret = -1;
	if (a_power_supply_dev->file_status != -1) {
		ret =
		    as_read_buffer(a_power_supply_dev->file_status, a_buffer,
				   a_length);
		if (ret < 0) {
			ERROR("Can't read input file\n");
		} else {
			if (ret < a_length)
				a_buffer[ret] = 0;
			else
				a_buffer[a_length - 1] = 0;
		}
	}

	return ret;
}

/*------------------------------------------------------------------------------*/

int32_t as_power_supply_get_capacity(struct as_power_supply_device *
				     a_power_supply_dev)
{
	int32_t value;
	int32_t ret;

	value = -1;
	if (a_power_supply_dev->file_capacity != -1) {
		ret = as_read_int(a_power_supply_dev->file_capacity, &value);
		if (ret < 0) {
			ERROR("Can't read input file\n");
			value = -1;
		}
	}

	return value;
}

/*------------------------------------------------------------------------------*/
