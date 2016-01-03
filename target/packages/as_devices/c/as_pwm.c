/*
** AsDevices - PWM access functions
**
**    Copyright (C) 2009-2010  The armadeus systems team
**    Fabien Marteau <fabien.marteau@armadeus.com>
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

/* #define DEBUG */
#include "as_helpers.h"
#include "as_pwm.h"

#define PWM_SYS_PATH   "/sys/class/pwm/pwm"
#define FREQUENCY_PATH "frequency"
#define PERIOD_PATH    "period"
#define DUTY_PATH      "duty"
#define ACTIVE_PATH    "active"

#define SIZE_OF_BUFF 50

/*------------------------------------------------------------------------------*/

struct as_pwm_device *as_pwm_open(int aPwmNumber)
{
	char buffer[SIZE_OF_BUFF];
	struct as_pwm_device *dev;

	dev = (struct as_pwm_device *)malloc(sizeof(struct as_pwm_device));
	if (dev == NULL) {
		ERROR("Can't allocate memory for pwm device structure\n");
		return NULL;
	}

	/* open pwm management files */
	snprintf(buffer, SIZE_OF_BUFF, "%s%d/%s", PWM_SYS_PATH, aPwmNumber,
		 FREQUENCY_PATH);
	if ((dev->fileFrequency = open(buffer, O_RDWR)) < 0) {
		ERROR("Can't open frequency files");
		return NULL;
	}
	snprintf(buffer, SIZE_OF_BUFF, "%s%d/%s", PWM_SYS_PATH, aPwmNumber,
		 PERIOD_PATH);
	if ((dev->filePeriod = open(buffer, O_RDWR)) < 0) {
		ERROR("Can't open period files");
		return NULL;
	}
	snprintf(buffer, SIZE_OF_BUFF, "%s%d/%s", PWM_SYS_PATH, aPwmNumber,
		 DUTY_PATH);
	if ((dev->fileDuty = open(buffer, O_RDWR)) < 0) {
		ERROR("Can't open duty files");
		return NULL;
	}
	snprintf(buffer, SIZE_OF_BUFF, "%s%d/%s", PWM_SYS_PATH, aPwmNumber,
		 ACTIVE_PATH);
	if ((dev->fileActive = open(buffer, O_RDWR)) < 0) {
		ERROR("Can't open active files");
		return NULL;
	}

	return dev;
}

/*------------------------------------------------------------------------------*/

int32_t as_pwm_set_frequency(struct as_pwm_device * aDev, int freq)
{
	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	return as_write_buffer(aDev->fileFrequency, freq);
}

/*------------------------------------------------------------------------------*/

int32_t as_pwm_get_frequency(struct as_pwm_device * aDev)
{
	char buffer[SIZE_OF_BUFF];
	int ret;

	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	ret = as_read_buffer(aDev->fileFrequency, buffer, SIZE_OF_BUFF);
	if (ret < 0) {
		ERROR("Can't read frequency\n");
		return ret;
	}

	return atoi(buffer);
}

/*------------------------------------------------------------------------------*/

int32_t as_pwm_set_period(struct as_pwm_device * aDev, int period)
{
	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	return as_write_buffer(aDev->filePeriod, period);
}

/*------------------------------------------------------------------------------*/

int32_t as_pwm_get_period(struct as_pwm_device * aDev)
{
	char buffer[SIZE_OF_BUFF];
	int ret;

	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	ret = as_read_buffer(aDev->filePeriod, buffer, SIZE_OF_BUFF);
	if (ret < 0) {
		ERROR("Can't read period\n");
		return ret;
	}

	return atoi(buffer);
}

/*------------------------------------------------------------------------------*/

int32_t as_pwm_set_duty(struct as_pwm_device * aDev, int duty)
{
	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	return as_write_buffer(aDev->fileDuty, duty);
}

/*------------------------------------------------------------------------------*/

int32_t as_pwm_get_duty(struct as_pwm_device * aDev)
{
	char buffer[SIZE_OF_BUFF];
	int ret;

	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	ret = as_read_buffer(aDev->fileDuty, buffer, SIZE_OF_BUFF);
	if (ret < 0) {
		ERROR("Can't read duty\n");
		return ret;
	}

	return atoi(buffer);
}

/*------------------------------------------------------------------------------*/

int32_t as_pwm_set_state(struct as_pwm_device * aDev, int enable)
{
	int ret = 0;

	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	if (enable) {
		ret = as_write_buffer(aDev->fileActive, 1);
	} else {
		ret = as_write_buffer(aDev->fileActive, 0);
	}
	if (ret < 0) {
		ERROR("Can't write to pwmX/active\n");
	}

	return ret;
}

/*------------------------------------------------------------------------------*/

int32_t as_pwm_get_state(struct as_pwm_device * aDev)
{
	char buffer[SIZE_OF_BUFF];
	int ret;

	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	ret = as_read_buffer(aDev->fileActive, buffer, SIZE_OF_BUFF);
	if (ret < 0) {
		ERROR("Can't read state\n");
		return ret;
	}

	return atoi(buffer);
}

/*------------------------------------------------------------------------------*/

int32_t as_pwm_close(struct as_pwm_device * aDev)
{
	int ret = 0;

	if (aDev == NULL) {
		ERROR("Device is NULL\n");
		return -1;
	}

	/* Close pwm management files */
	ret = close(aDev->fileFrequency);
	if (ret < 0) {
		ERROR("Can't close /frequency");
		return ret;
	}
	ret = close(aDev->filePeriod);
	if (ret < 0) {
		ERROR("Can't close /period");
		return ret;
	}
	ret = close(aDev->fileDuty);
	if (ret < 0) {
		ERROR("Can't close /duty");
		return ret;
	}
	ret = close(aDev->fileActive);
	if (ret < 0) {
		ERROR("Can't close /active");
		return ret;
	}

	return ret;
}
