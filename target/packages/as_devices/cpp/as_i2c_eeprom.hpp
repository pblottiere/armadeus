/*
**    THE ARMadeus Systems
**
**    Copyright (C) 2012  The armadeus systems team
**    Jérémie Scheer <jeremie.scheeer@armadeus.com>
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
**
*/

/** @file
 * @brief Provide C++ wrapper for I2C eeprom device
 *
 * @ingroup cpp_wrappers
 */

#ifndef AS_I2C_EEPROM__HPP__
#define AS_I2C_EEPROM__HPP___

#include "as_i2c_eeprom.h"

/**
  * C++ wrapper for I2C EEPROM device
  *
  * @ingroup cpp_wrappers
  */
class AsI2CEeprom
{
public:
	/**
	  * Constructor
	  */
	AsI2CEeprom(int aBusNumber, unsigned long anEepromAddr, unsigned long anAddrWidth, \
unsigned long aPageBufferSize, unsigned int aNumberPages, unsigned int aPageSize);
	virtual ~AsI2CEeprom();

	/** @brief Read an EEPROM page
	  *
	  * @param aPageNumber page to read on
	  * @param aBuffer data buffer
	 */
	long read(unsigned int aPageNumber, unsigned char* aBuffer);

	/** @brief Write on an EEPROM page
	  *
	  * @param aPageNumber page to write on
	  * @param aBuffer data buffer
	  * @param aBufferSize data buffer size
	  */
	long write(unsigned int aPageNumber, unsigned char* aBuffer, unsigned int aBufferSize);

	/** @brief Change write time delay
	  *
	  * @param aWriteTime delay between 2 write cmd in micro seconds
	 */
	long set_write_time(unsigned int aWriteTime);

	/** @brief Read from EEPROM non sequential
	  *
	  * @param anAddress address to start write
	  * @param aLength length of data to write
	  * @param aBuffer buffer containing data to write
	 */
	long read_mid(unsigned int anAddress, unsigned int aLength, unsigned char* aBuffer);

	/** @brief Write on an EEPROM non sequential
	  *
	  * @param anAddress address to start read
	  * @param aLength length of data to read
	  * @param aBuffer buffer receiving data
	  */
	long write_mid(unsigned int anAddress, unsigned int aLength, unsigned char* aBuffer);

protected:
    /**
     * I2C eeprom device C structure
     */
	mutable struct as_i2c_eeprom_device *mDev;
};

#endif // AS_I2C_EEPROM__HPP__
