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

/** @file
 * @brief Provide standard commands to drive TCS3274 device
 *
 * @ingroup c_files
 */

#ifndef __AS_TCS3472_H__
#define __AS_TCS3472_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "as_i2c.h"

/** @brief Structure to handle TCS3274 device
 *
 * @ingroup c_files
 */
struct as_tcs3472 {
	struct as_i2c_device *i2c_dev;	/*!< I2C bus handler */
};

/** @brief Structure to store color results from TCS3274
 *
 */
struct as_tcs3472_colours {
	int clear;	/*!< Clear component */
	int red;	/*!< Red component */
	int green;	/*!< Green component */
	int blue;	/*!< Blue component */
};

/** @brief Initialize access to a TCS3472* chip
 *
 * @param i2c_bus I2C bus number
 * @param i2c_addr I2C chip address
 *
 * @return as_tcs3472_device structure on success, NULL on error
 */
struct as_tcs3472 *as_tcs3472_open(int i2c_bus, int i2c_addr);

/** @brief Start a color convertion
 *
 * @param tcs3472 the TCS3472 to ask for
 * @param time duration of the convertion (integration time)
 *
 * @return negative value on error
 */
int32_t as_tcs3472_start_rgbc(struct as_tcs3472 *tcs3472, int time);

/** @brief Get R/G/B/C colour components from previous conversion
 *
 * @param tcs3472 the TCS3472 to ask for
 * @param colours pointer to the structure to store the value
 *
 * @return negative value on error
 */
int32_t as_tcs3472_get_colours(struct as_tcs3472 *tcs3472,
			       struct as_tcs3472_colours *colours);

/** @brief Close TCS3472 access
 *
 * @param tcs3472 the chip to close access for
 *
 * @return negative value on error
 */
int32_t as_tcs3472_close(struct as_tcs3472 *tcs3472);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __AS_TCS3472_H__ */
