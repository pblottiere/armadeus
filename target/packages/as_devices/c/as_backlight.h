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

/** @file
 * @brief Provide standard commands to drive backlight
 *
 * @ingroup c_files
 */

#ifndef __ASBACKLIGHT_H__
#define __ASBACKLIGHT_H__

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#include <stdint.h>

/** @brief Structure to store backlight device parameters
 *
 * @ingroup c_files
 */
struct as_backlight_device {
	int fileActualBrightness;	/*!< file descriptor to get current brightness */
	int fileMaxBrightness;	/*!< file descriptor to get maximum brightness */
	int fileBrightness;	/*!< file descriptor to set brightness */
};

/** @brief Open Backlight device
 *
 * @param aBacklightName name of backlight to use (imx-bl if NULL)
 *
 * @return pointer to device structure, NULL if error
 */
struct as_backlight_device *as_backlight_open2(char *aBacklightName);

/** @brief Open Backlight device
 *
 * @return result of as_backlight_open2(NULL)
 */
struct as_backlight_device *as_backlight_open();

/** @brief Get actual brightness
 *
 * @param aDev pointer to device structure
 *
 * @return actual brightness value, negative value on error
 */
int32_t as_backlight_get_actual_brightness(struct as_backlight_device
					   *aDev);

/** @brief Get maximum brightness
 *
 * @param aDev pointer to device structure
 *
 * @return maximum allowed brightness, negative value on error
 */
int32_t as_backlight_get_max_brightness(struct as_backlight_device
					*aDev);

/** @brief Set brightness
 *
 * @param aDev          pointer to device structure
 * @param aBrightness   backlight brightness
 *
 * @return negative value on error
 */
int32_t as_backlight_set_brightness(struct as_backlight_device *aDev,
				    int aBrightness);

/** @brief Close backlight device
 *
 * @param aDev pointer structure to device
 *
 * @return negative value on error
 */
int32_t as_backlight_close(struct as_backlight_device *aDev);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __ASBACKLIGHT_H__ */
