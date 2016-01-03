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
 * @brief Provide C++ wrapper for DAC device
 *
 * @ingroup cpp_wrappers
 */

#ifndef AS_DAC__HPP__
#define AS_DAC__HPP__

#include "as_dac.h"

/**
  * C++ wrapper for DAC device
  *
  * @ingroup cpp_wrappers
  */
class AsDac
{
public:
    /** @brief Constructor: Open DAC
     *
     * @param aDacType type name of adc ("max5821" or "mcp4912")
     * @param aBusNumber bus number used
     * @param aAddress i2c address for i2c chip, and CS number for SPI chip
     * @param aVRef voltage reference in milivolt
     */
	AsDac(const char *aDacType, int aBusNumber, int aAddress, int aVRef);
	virtual ~AsDac();	
	
    /** @brief set a channel value.
     *
     * @param aChannel channel number (A:0, B:1,...)
     * @param aValue channel value in milivolt
     *
     * @return negative value on error
     */
	long setValueInMillivolts(int aChannel, int aValue);

protected:
    /**
     * DAC device C structure
     */
	mutable struct as_dac_device *mDev;
};

#endif // AS_DAC__HPP__
