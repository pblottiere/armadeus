/*
**    THE ARMadeus Systems
**
**    Copyright (C) 2012  The armadeus systems team
**    J�r�mie Scheer <jeremie.scheer@armadeus.com>
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* for open()   */
#include <unistd.h>		/* for write()  */
#include <limits.h>

#define DEBUG
#include "as_helpers.h"
#include "as_backlight.h"

#define BACKLIGHT_SYS_PATH      "/sys/class/backlight"
#define BACKLIGHT_DEFAULT       "imx-bl"
#define ACTUAL_BRIGHTNESS_PATH  "actual_brightness"
#define MAX_BRIGHTNESS_PATH     "max_brightness"
#define BRIGHTNESS_PATH         "brightness"

/*------------------------------------------------------------------------------*/

struct as_backlight_device *as_backlight_open2(char *aBacklightName)
{
	char buffer[PATH_MAX];
	struct as_backlight_device *dev;
	char *fake_root;

	fake_root = as_helpers_get_root();

	if (aBacklightName == NULL) {
		aBacklightName = BACKLIGHT_DEFAULT;
	}

	dev =
	    (struct as_backlight_device *)
	    malloc(sizeof(struct as_backlight_device));
	if (dev == NULL) {
		ERROR("Can't allocate memory for backlight device structure\n");
		return NULL;
	}

	/* open backlight management files */
	snprintf(buffer, sizeof(buffer), "%s%s/%s/%s", fake_root,
		 BACKLIGHT_SYS_PATH, aBacklightName, ACTUAL_BRIGHTNESS_PATH);
	if ((dev->fileActualBrightness = open(buffer, O_RDONLY)) < 0) {
		ERROR("Can't open %s file\n", buffer);
		return NULL;
	}
	snprintf(buffer, sizeof(buffer), "%s%s/%s/%s", fake_root,
		 BACKLIGHT_SYS_PATH, aBacklightName, MAX_BRIGHTNESS_PATH);
	if ((dev->fileMaxBrightness = open(buffer, O_RDONLY)) < 0) {
		ERROR("Can't open %s file\n", buffer);
		return NULL;
	}
	snprintf(buffer, sizeof(buffer), "%s%s/%s/%s", fake_root,
		 BACKLIGHT_SYS_PATH, aBacklightName, BRIGHTNESS_PATH);
	if ((dev->fileBrightness = open(buffer, O_RDWR)) < 0) {
		ERROR("Can't open %s file\n", buffer);
		return NULL;
	}

	return dev;
}

struct as_backlight_device *as_backlight_open()
{
	return as_backlight_open2(NULL);
}

/*------------------------------------------------------------------------------*/

int32_t as_backlight_get_actual_brightness(struct as_backlight_device * aDev)
{
	char buffer[32];
	int ret;

	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	ret =
	    as_read_buffer(aDev->fileActualBrightness, buffer, sizeof(buffer));
	if (ret < 0) {
		ERROR("Can't read actual brightness");
		return ret;
	}

	return atoi(buffer);
}

/*------------------------------------------------------------------------------*/

int32_t as_backlight_get_max_brightness(struct as_backlight_device * aDev)
{
	char buffer[32];
	int ret;

	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	ret = as_read_buffer(aDev->fileMaxBrightness, buffer, sizeof(buffer));
	if (ret < 0) {
		ERROR("Can't read max brightness");
		return ret;
	}

	return atoi(buffer);
}

/*------------------------------------------------------------------------------*/

int32_t as_backlight_set_brightness(struct as_backlight_device * aDev,
				    int aBrightness)
{
	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	return as_write_buffer(aDev->fileBrightness, aBrightness);
}

/*------------------------------------------------------------------------------*/

int32_t as_backlight_close(struct as_backlight_device * aDev)
{
	int ret = 0;

	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	/* Close backlight management files */
	ret = close(aDev->fileActualBrightness);
	if (ret < 0) {
		ERROR("Can't close /actual_brightness");
		return ret;
	}
	ret = close(aDev->fileMaxBrightness);
	if (ret < 0) {
		ERROR("Can't close /max_brightness");
		return ret;
	}
	ret = close(aDev->fileBrightness);
	if (ret < 0) {
		ERROR("Can't close /brightness");
		return ret;
	}

	free(aDev);

	return ret;
}
