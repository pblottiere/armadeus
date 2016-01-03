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
 * Copyright (C) 2011-2014 The Armadeus Project & Armadeus Systems teams
 * Author: Fabien Marteau <fabien.marteau@armadeus.com>
 *
 */

/** @file
 * @brief Provide standard commands to drive DAC
 *
 * @ingroup c_files
 */

#ifndef __AS_DAC_H__
#define __AS_DAC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AS_MAX5821_TYPE "max5821"
#define AS_MCP4912

/** @brief Store DAC parameters
 *
 * @ingroup c_files
 */
struct as_dac_device {
	int bus_number;		/*!< bus number */
	int chip_address;	/*!< chip address on bus */
	int vref;		/*!< reference voltage */
	const char *device_type;/*!< device type: "max5821" or "mcp4912" */
	void *chip_param;	/*!< private data for chip handler */
};

/** @brief Initialize DAC chip access
 *
 * @param dac_type type of DAC ("max5821" or "mcp4912")
 * @param bus bus number used (I2C/SPI)
 * @param address I2C address for I2C chip, and CS number for SPI chip
 * @param vref voltage reference in milivolt
 *
 * @return as_dac_device structure pointer on success, NULL on error
 */
struct as_dac_device *as_dac_open(const char *dac_type, int bus,
				  int address, int vref);

/** @brief Set a DAC channel to given value
 *
 * @param dac handler of the DAC device
 * @param channel channel number (A:0, B:1,...)
 * @param value channel value in milivolt
 *
 * @return negative value on error
 */
int32_t as_dac_set_value_in_millivolts(struct as_dac_device *dac,
				       int channel, int value);

/** @brief Close DAC device access
 *
 * @param dac handler of DAC device
 *
 * @return negative value on error
 */
int32_t as_dac_close(struct as_dac_device *dac);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __AS_DAC_H__ */
