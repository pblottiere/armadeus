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
 * @brief Provide C++ wrapper for SPI device
 *
 * @ingroup cpp_wrappers
 */

#ifndef AS_SPI__HPP__
#define AS_SPI__HPP__

#include <cstring>

#include "as_spi.h"

/**
  * C++ wrapper for SPI bus device
  *
  * @ingroup cpp_wrappers
  */
class AsSpi
{
public:
    /** @brief Constructor: Open a SPI bus.
     *
     * @param aSpidev_name path name
     */
	AsSpi(const unsigned char *aSpidev_name);
	virtual ~AsSpi();

    /** @brief Set bits order
     *
     * @param aLsb if 1 lsb first, else msb first
     *
     * @return lsb first if positive, msb first if 0, negative value on error
     */
	int setLsb(unsigned char aLsb);

    /** @brief Get bits order
     *
     * @return lsb first if positive, msb first if 0, negative value on error
     */
	int getLsb() const;

    /** @brief Set spi bus mode
     *
     * @param aMode	spi's bus mode
     *
     * @return mode if positive value, negative value on error
     */
	int setMode(unsigned char aMode);

    /** @brief Get spi bus mode
     *
     * @return mode if positive value, negative value on error
     */
	int getMode() const;

    /** @brief Set clock bus speed
     *
     * @param aSpeed clock speed
     *
     * @return speed in Hz, negative value on error
     */
	int setSpeed(unsigned char aSpeed);

    /** @brief Get clock bus speed
     *
     * @return speed in Hz
     */
	int getSpeed() const;

    /** @brief Get bits per word
     *
     * @return bit per word if positive value, negative value on error
     */
	int getBitsPerWord() const;

    /** @brief Set bits per word
     *
     * @param aBits number of bits per word
     *
     * @return bit per word if positive value, negative value on error
     */
	int setBitsPerWord(unsigned char aBits);

    /** @brief Forge arbitrary length message (31bits max) and send it
     *
     * @param aMsg right adjusted message
     * @param aLen lenght of the message
     * @param aSpeed clock speed in Hz
     *
     * @return message read
     */
	unsigned long long msg(unsigned long long aMsg, size_t aLen, unsigned long aSpeed);

protected:
    /**
     * SPI device file descriptor
     */
	int mFd;
};

#endif // AS_SPI__HPP__
