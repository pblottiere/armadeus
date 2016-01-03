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
 * Copyright (C) 2009 Fabien Marteau <fabien.marteau@armadeus.com> 
 *
 */

#ifndef AS_93LCXX_H_
#define AS_93LCXX_H_

/** @file
 * @brief Provide standard commands to drive 93LCxx eeprom (from microchip)
 *
 * @ingroup c_files
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <sys/types.h>
#include <stdio.h>

/** 
 * Store eeprom parameters
 *
 * @ingroup c_files
 */
struct as_93lcxx_device {
	unsigned char *spidev_filename;	    /**< filename of the spidev /dev file */
	uint8_t type;	  /**< type of the eeprom : 46, 56 or 66 (see DS) */
	uint32_t speed;	  /**< SCLK speed, in Hz */
	uint8_t word_size;     /**< word size for transaction, 8 or 16 */
	int fd;	      /**< File handler for spidev bus */
};

/** @brief open the device
 *
 * @param aSpidev_filename path to spidev
 * @param aType of the eeprom
 * @param aSpeed clock speed in Hz
 * @param aWord_size word size for transaction
 *
 * @return as_93lcxx_device struct pointer on success, NULL on error
 */
struct as_93lcxx_device *as_93lcxx_open(unsigned char *aSpidev_filename,
					uint8_t aType,
					uint32_t aSpeed,
					uint8_t aWord_size);

/** @brief close the device
 *
 * @param aDev spidev structure
 *
 */
void as_93lcxx_close(struct as_93lcxx_device *aDev);

/** @brief read a value in address given
 *
 * @param aDev 93LCxx structure
 * @param aAddress addresse of register to be read
 *
 * @return positive register value on success, negative value on error.
 */
int32_t as_93lcxx_read(struct as_93lcxx_device *aDev,
		       uint16_t aAddress);

/** @brief enable write on eeprom
 *
 * @param aDev 93LCxx structure
 *
 * @return positive value on success, negative value on error
 */
int32_t as_93lcxx_ewen(struct as_93lcxx_device *aDev);

/** @brief Force all data bits of the specified iaddress to 1
 *
 * @param aDev 93LCxx structure
 * @param aAddress addresse of register to be erased
 *
 * @return positive register value on success, negative value on error.
 */
int32_t as_93lcxx_erase(struct as_93lcxx_device *aDev,
			uint16_t aAddress);

/** @brief Force all bits in eeprom to 1
 *
 * @param aDev 93LCxx structure
 *
 * @return positive register value on success, negative value on error.
 */
int32_t as_93lcxx_erase_all(struct as_93lcxx_device *aDev);

/** @brief write a value in given address
 *
 * @param aDev 93LCxx structure
 * @param aAddress addresse of register to be read
 * @param aValue value to be wrote
 *
 * @return positive register value on success, negative value on error.
 */
int32_t as_93lcxx_write(struct as_93lcxx_device *aDev,
			uint16_t aAddress, uint16_t aValue);

/** @brief write the entire memory array with value given
 *
 * @param aDev 93LCxx structure
 * @param aValue value to be wrote
 *
 * @return positive register value on success, negative value on error.
 */
int32_t as_93lcxx_write_all(struct as_93lcxx_device *aDev,
			    uint16_t aValue);

/** @brief disable write on eeprom
 *
 * @param aDev 93LCxx structure
 *
 * @return positive value on success, negative value on error
 */
int32_t as_93lcxx_ewds(struct as_93lcxx_device *aDev);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* AS_93LCXX_H_ */
