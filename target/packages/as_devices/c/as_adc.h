/*
**    The ARMadeus Project
**
**    Copyright (C) 2011  The armadeus systems team
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

/** @file
 * @brief Provide standard commands to drive ADC
 *
 * @ingroup c_files
 */

#ifndef __AS_ADC_H__
#define __AS_ADC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

extern const char AS_MAX1027_NAME[];
extern const char AS_AS1531_NAME[];
extern const char AS_IIO_NAME[];

/** @brief Store ADC parameters
 *
 * @ingroup c_files
 */
struct as_adc_device {
	const char *device_type;	/*!< device type: "as1531" or "max1027" */
	int device_num;	/*!< device id on bus */
	int vref;	/*!< reference voltage in millivolts */
	void *chip_param;	/*!< private data for chip handler */
};

/** @brief Initialize ADC chip access
 *
 * @param device_num device number
 * @param type ADC type: "max1027"/"as1531"/"iio"
 * @param vref reference voltage in millivolts
 *
 * @return error if negative value
 */
struct as_adc_device *as_adc_open(const char *type, int device_num,
				  int vref);

/** @brief Get given ADC channel value in millivolts
 *
 * @param adc ADC handler
 * @param channel channel to use
 *
 * @return millivolts or error if negative
 */
int32_t as_adc_get_value_in_millivolts(struct as_adc_device *adc,
				       int channel);

/** @brief Close given ADC access
 *
 * @param adc ADC handler
 *
 * @return error if negative
 */
int32_t as_adc_close(struct as_adc_device *adc);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __AS_ADC_H__ */
