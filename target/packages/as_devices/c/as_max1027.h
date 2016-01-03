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
 * Copyright (C) 2010 Fabien Marteau <fabien.marteau@armadeus.com>
 *
 */

/** @file
 * @brief Provide standard commands to drive MAX1027
 *
 * @ingroup c_files
 */

#ifndef __AS_MAX1027_H__
#define __AS_MAX1027_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "as_adc.h"
#define NUMBER_OF_CHANNELS	(7)
#define PATH_SIZE		(50)

typedef enum {
	AS_MAX1027_FAST,	/*!< Fast mode */
	AS_MAX1027_SLOW	/*!< Slow mode */
} AS_max1027_mode;

/** @brief Structure to store max1027 parameters
 *
 * @ingroup c_files
 */
struct as_max1027_device {
	AS_max1027_mode mode;	/*!< mode/speed */
	uint8_t scan_mode;	/*!< ?? */
	char path[PATH_SIZE];	/*!< ?? */
};

/** @brief Initialize max1027 access
 *
 * @param num device number
 * @param vref reference voltage
 *
 * @return pointer to device structure, NULL if error
 */
struct as_adc_device *as_adc_open_max1027(int num, int vref);

/** @brief Get given max1027 channel voltage value
 *
 * @param max1027 chip handler
 * @param channel channel to sample
 *
 * @return negative if error
 */
int32_t as_adc_get_value_in_millivolts_max1027(struct as_adc_device
					       *max1027, int channel);

/** @brief Close given max1027 access
 *
 * @param max1027 chip handler
 *
 * @return negative if error
 */
int32_t as_adc_close_max1027(struct as_adc_device *max1027);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __AS_MAX1027_H__ */
