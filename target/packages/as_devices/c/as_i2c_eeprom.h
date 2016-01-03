/*
**    THE ARMadeus Systems
**
**    Copyright (C) 2012  The armadeus systems team
**    Jérémie Scheer <jeremie.scheer@armadeus.com>
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

#ifndef AS_I2C_EEPROM_H_
#define AS_I2C_EEPROM_H_

/** @file
 * @brief Provide standard commands to drive i2c eeprom (from microchip)
 *
 * @ingroup c_files
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "as_i2c.h"

/** 
 * Store eeprom parameters
 *
 * @param i2c_dev   store i2c device parameters
 *
 * @ingroup c_files
 */
struct as_i2c_eeprom_device {
    /**
     * Store i2c device parameters
     */
	struct as_i2c_device *i2c_dev;
    /**
     * Eeprom memory address
     */
	uint32_t eeprom_addr;
    /**
     * Eeprom memory address width
     */
	uint32_t addr_width;
    /**
     * Eeprom page buffer size
     */
	uint32_t page_buffer_size;
    /**
     * Number of pages in eeprom memory
     */
	uint16_t number_pages;
    /**
     * Page size in eeprom memory
     */
	uint16_t page_size;
    /**
     * Writing time in us
     */
	uint32_t write_time_us;
};

/** @brief open the device
  *
  * @param aBusNumber number of I2C bus used by EEPROM
  * @param anEeprom_addr EEPROM address on I2C bus
  * @param anAddr_width width of EEPROM address
  * @param aPage_buffer_size size of the page write buffer
  * @param aNumber_pages number of pages in EEPROM
  * @param aPage_size page size
  */
struct as_i2c_eeprom_device *as_i2c_eeprom_open(int aBusNumber,
						uint32_t anEeprom_addr,
						uint32_t anAddr_width,
						uint32_t
						aPage_buffer_size,
						uint16_t aNumber_pages,
						uint16_t aPage_size);

/** @brief close the device
 *
 * @param aDev spidev structure
 *
 */
void as_i2c_eeprom_close(struct as_i2c_eeprom_device *aDev);

/** @brief Read an EEPROM page
  *
  * @param aDev EEPROM structure
  * @param aPage_number page to read on
  * @param aBuffer data buffer
 */
int32_t as_i2c_eeprom_read(struct as_i2c_eeprom_device *aDev,
			   uint16_t aPage_number, uint8_t * aBuffer);

/** @brief Write on an EEPROM page
  *
  * @param aDev EEPROM structure
  * @param aPage_number page to write on
  * @param aBuffer data buffer
  * @param aBuffer_size data buffer size
  */
int32_t as_i2c_eeprom_write(struct as_i2c_eeprom_device *aDev,
			    uint16_t aPage_number, uint8_t * aBuffer,
			    uint16_t aBuffer_size);

/** @brief Change write time delay
  *
  * @param aDev EEPROM structure
  * @param aWriteTime delay between 2 write cmd in micro seconds
 */
int32_t as_i2c_eeprom_set_write_time(struct as_i2c_eeprom_device *aDev,
				     uint32_t aWriteTime);

/** @brief Write on an EEPROM non sequential
  *
  * @param aDev EEPROM structure
  * @param aAddr address to start write
  * @param aLen length of data to write
  * @param aBuffer buffer containing data to write
  */
int32_t as_i2c_eeprom_write_mid(struct as_i2c_eeprom_device *aDev,
				uint16_t aAddr, uint16_t aLen,
				uint8_t * aBuffer);

/** @brief Read from an EEPROM non sequential
  *
  * @param aDev EEPROM structure
  * @param aAddr address to start read
  * @param aLen length of data to read
  * @param aBuffer buffer receiving data
  */
int32_t as_i2c_eeprom_read_mid(struct as_i2c_eeprom_device *aDev,
			       uint16_t aAddr, uint16_t aLen,
			       uint8_t * aBuffer);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* AS_I2C_EEPROM_H_ */
