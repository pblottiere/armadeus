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
 */

/** @file
 * @brief Provide standard commands to drive ADC IIO
 *
 * @ingroup c_files
 */

#ifndef __AS_ADC_IIO_H__
#define __AS_ADC_IIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "as_adc.h"

/** @brief Initialize ADC access
 *
 * @param dev_num device number for IIO infra
 * @param vref reference voltage
 */
struct as_adc_device *as_adc_open_iio(int dev_num, int vref);

/** @brief Get given IIO ADC channel voltage
 *
 * @param adc ADC handler returned by as_adc_open_iio()
 * @param channel channel to sample
 */
int32_t as_adc_get_value_in_millivolts_iio(struct as_adc_device *adc,
					   int channel);

/** @brief Close ADC access
 *
 * @param adc ADC handler returned by as_adc_open_iio()
 */
int32_t as_adc_close_iio(struct as_adc_device *adc);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __AS_ADC_IIO_H__ */
