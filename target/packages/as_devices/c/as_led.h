/*
**    The Armadeus Project
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
 * @brief Provide standard commands to drive LEDs
 *
 * @ingroup c_files
 */

#ifndef __ASLED_H__
#define __ASLED_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/**
 * @brief Store LED device parameters
 *
 * @ingroup c_files
 */
struct as_led_device {
	char led_name[32];	/*!< LED device name */
	int file_brightness;	/*!< file descriptor to set LED brightness */
	int file_max_brightness;	/*!< file descriptor to get LED max possible brightness */
};

/** @brief Initialize LED access
 *
 * @param led_name char* name of the led
 *
 * @return NULL on error
 */
struct as_led_device *as_led_open(char *led_name);

/** @brief Close led access
 *
 * @param led LED handler returned by as_led_open()
 *
 * @return error if negative
 */
int32_t as_led_close(struct as_led_device *led);

/** @brief  Get max brightness
 *
 * @param led LED handler returned by as_led_open()
 *
 * @return maximum allowed brightness, negative value on error
 */
int32_t as_led_get_max_brightness(struct as_led_device *led);

/** @brief  Set brightness
 *
 * @param led LED handler returned by as_led_open()
 * @param a_brightness uint32_t requested value for led brighthness
 *
 * @return current brightness, negative value on error
 */
int32_t as_led_set_brightness(struct as_led_device *led,
			      uint32_t a_brightness);

/** @brief  Get brightness
 *
 * @param led LED handler returned by as_led_open()
 *
 * @return current brightness, negative value on error
 */
int32_t as_led_get_brightness(struct as_led_device *led);

/** @brief  Make a led blinking
 *
 * @param led LED handler returned by as_led_open()
 * @param a_delay_on uint32_t value for led on time
 * @param a_delay_off uint32_t value for led off time
 *
 * @return negative value on error
 */
int32_t as_led_start_blinking(struct as_led_device *led,
			      int32_t a_delay_on, int32_t a_delay_off);

/** @brief  Led stop blinking
 *
 * @param led LED handler returned by as_led_open()
 *
 * @return negative value on error
 */
int32_t as_led_stop_blinking(struct as_led_device *led);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __ASLED_H__ */
