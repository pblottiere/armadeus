/*
**    THE ARMadeus Systems
** 
**    Copyright (C) 2011  The armadeus systems team 
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
 * @brief Provide C++ wrapper for I2C device
 *
 * @ingroup cpp_wrappers
 */

#ifndef AS_I2C__HPP__
#define AS_I2C__HPP__

#include <cstring>

#include "as_i2c.h"

/**
  * C++ wrapper for I2C bus device
  * 
  * @ingroup cpp_wrappers
  */
class AsI2c
{
public:
    /** @brief Constructor: Open an I2C bus.
     *
     * @param aBusNumber	bus to open
     */
	AsI2c(int aBusNumber);
	virtual ~AsI2c();

    /** @brief Set chip's I2C slave address 
     *
     * @param aAddr		slave's address
     *
     * @return error if negative
     */
	long setSlaveAddr(unsigned char aAddr);

    /** @brief Get chip's I2C slave address 
     *
     * @return slave's address
     */
	long getSlaveAddr() const;

    /** @brief Read several bytes (ioctl() method) from given chip.
     *
     * @param aData		read data
     * @param aSize		data size
     *
     * @return error if negative
     */
	long read(unsigned char *aData, size_t aSize) const;
    
    /** @brief Write several bytes (ioctl() method) to given chip.
     *
     * @param aData		data to write
     * @param aSize		data size
     *
     * @return error if negative
     */
	long write(unsigned char *aData, size_t aSize);

    /** @brief Read from given chip at a given register address (ioctl() method).
     *
     * @param aReg		register address
     * @param aData		read data
     * @param aSize		data size
     *
     * @return error if negative
     */
	long readReg(unsigned char aReg, unsigned char *aData, size_t aSize) const;

    /** @brief Write to given chip at a given register address (ioctl() method).
     *
     * @param aReg		register address
     * @param aData		data to write
     * @param aSize		data size
     *
     * @return error if negative
     */
	long writeReg(unsigned char aReg, unsigned char *aData, size_t aSize);

    /** @brief forge a read message like this:
     * S Addr[W] wdata0 [A] wdata1 [A] ... RS Addr R [rdata0] A [rdata1] A ... P
     *
     * @param aWData	    data to write
     * @param aWriteSize    written data size
     * @param aRData	    read data
     * @param aReadSize	    read data size
     *
     * @return error if negative
     */
	long readMsg(unsigned char *aWData, unsigned char aWriteSize, unsigned char *aRData, size_t aReadSize);

    /** @brief Read a byte from the given register.
     *
     * @param aReg		register address
     *
     * @return read byte
     */
	long readRegByte(unsigned char aReg) const;

    /** @brief Write a byte to the given register.
     *
     * @param aReg		register address
     * @param aVal		byte to write
     *
     * @return error if negative
     */
	long writeRegByte(unsigned char aReg, unsigned char aVal);

protected:
    /**
     * I2C bus device C structure
     */
	mutable struct as_i2c_device *mDev;
};

#endif // AS_I2C__HPP__
