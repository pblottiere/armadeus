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
 * @brief Provide C++ wrapper for TCS3274 device
 *
 * @ingroup cpp_wrappers
 */

#ifndef AS_93LCXX__HPP__
#define AS_93LCXX__HPP__

#include "as_93lcxx.h"

/**
  * C++ wrapper for 93LCXX device
  *
  * @ingroup cpp_wrappers
  */
class As93LCXX
{
public:
	/**
	  * Constructor
	  *
	  * @param aSpidevFilename path to spidev
	  * @param aType of the eeprom
	  * @param aSpeed clock speed in Hz
	  * @param aWordSize word size for transaction
	  */
	As93LCXX(unsigned char *aSpidevFilename, unsigned char aType, unsigned long aSpeed, unsigned char aWordSize);
	virtual ~As93LCXX();

	/** @brief read a value in address given
	 *
	 * @param anAddress addresse of register to be read
	 *
	 * @return positive register value on success, negative value on error.
	 */
	long read(unsigned int anAddress) const;

	/** @brief enable write on eeprom
	 *
	 * @return positive value on success, negative value on error
	 */
	long ewen();

	/** @brief Force all data bits of the specified iaddress to 1
	 *
	 * @param anAddress addresse of register to be erased
	 *
	 * @return positive register value on success, negative value on error.
	 */
	long erase(unsigned int anAddress);

	/** @brief Force all bits in eeprom to 1
	 *
	 * @return positive register value on success, negative value on error.
	 */
	long eraseAll();

	/** @brief write a value in given address
	 *
	 * @param anAddress addresse of register to be read
	 * @param aValue value to be wrote
	 *
	 * @return positive register value on success, negative value on error.
	 */
	long write(unsigned int anAddress, unsigned int aValue);

	/** @brief write the entire memory array with value given
	 *
	 * @param aValue value to be wrote
	 *
	 * @return positive register value on success, negative value on error.
	 */
	long writeAll(unsigned int aValue);

	/** @brief disable write on eeprom
	 *
	 * @return positive value on success, negative value on error
	 */
	long ewds();

protected:
    /**
     * As_93lcxx device C structure
     */
	mutable struct as_93lcxx_device *mDev;
};

#endif // AS_93LCXX__HPP__
