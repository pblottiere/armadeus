/*
**    The ARMadeus Project
**
**    Copyright (C) 2013  The armadeus systems team
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

/** @file
 * @brief Provide standard commands to drive Power Supply
 *
 * @ingroup c_files
 */

#ifndef __ASPOWERSUPPLY_H__
#define __ASPOWERSUPPLY_H__

#include <stdint.h>		/* uint32_t */

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/**
 * @brief Store power supplier parameters
 *
 * @ingroup c_files
 */
struct as_power_supply_device {
	char power_supply_name[32];	/*!< device name */
	int file_online;
	int file_capacity;
	int file_status;
};

/** @brief Initialize power supply access
 *
 * @param a_power_supply_name char* name of the power supplier
 *
 * @return NULL on error
 */
struct as_power_supply_device *as_power_supply_open(char
						    *a_power_supply_name);

/** @brief Close power supply access
 *
 * @param a_power_supply_dev as_power_supply_device pointer structure
 *
 * @return error if negative
 */
int32_t as_power_supply_close(struct as_power_supply_device *a_led_dev);

/** @brief  Get online state
 *
 * @param a_power_supply_dev as_power_supply_device pointer structure
 *
 * @return 1 if power supply active, 0 otherwise
 */
int32_t as_power_supply_is_online(struct as_power_supply_device
				  *a_power_supply_dev);

/** @brief  Get online state
 *
 * @param a_power_supply_dev as_power_supply_device pointer structure
 * @param a_buffer char pointer to receive status as text
 * @param a_length uint32_t buffer size
 *
 * @return -1 on error, 0 on success
 */
int32_t as_power_supply_get_status(struct as_power_supply_device
				   *a_power_supply_dev, char *a_buffer,
				   uint32_t a_length);

/** @brief  Get capacity
 *
 * @param a_power_supply_dev as_power_supply_device pointer structure
 *
 * @return charge level in percent
 */
int32_t as_power_supply_get_capacity(struct as_power_supply_device
				     *a_power_supply_dev);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __ASPOWERSUPPLY_H__ */
