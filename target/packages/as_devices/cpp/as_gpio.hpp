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
 * @brief Provide C++ wrapper for GPIOs
 *
 * @ingroup cpp_wrappers
 */

#ifndef AS_GPIO__HPP__
#define AS_GPIO__HPP__

#include "as_gpio.h"

/**
  * C++ wrapper for GPIO device
  * 
  * @ingroup cpp_wrappers
  */
class AsGpio
{
public:
    /** @brief Constructor: Initialize port access
     *
     * @param aGpioNum gpio number
     *
     */
	AsGpio(int aGpioNum);
	virtual ~AsGpio();

    /** @brief  Set pin direction
     *
     * @param aDirection direction 0:input 1:output
     *
     * @return error if negative value
     */
	long setPinDirection(char* aDirection);

    /** @brief  Get pin direction
     *
     * @return pin direction if positive or null, error if negative
     */
	const char* getPinDirection() const;

    /** @brief Set pin value
     *
     * @param aValue value of pin (1 or 0)
     *
     * @return error if negative
     */
	long setPinValue(int aValue);

    /** @brief Get pin value
     *
     * @return pin value if positive or null, error if negative
     */
	long getPinValue() const;

    /** @brief Get pin value, blocking until an interrupt/event occurs
     *
     * @param aDelay_ms waiting delay in milliseconds
     *
     * @return pin value if positive or null, read error if -1, timeout if -10
     */
	long waitEvent(int aDelay_ms) const;
	
    /** @brief Set pin irq mode
     *
     * @param aMode irq mode
     *
     * @return error if negative
     */
	long setIrqMode(char* aMode);
    
    /** @brief Get pin irq mode value
     *
     * @return none/rising/falling/both
     */
	const char* getIrqMode() const;

    /** @brief Get pin number value
     *
     * @return pin number, error if negative
     */
	long getPinNum() const;

protected:
    /**
     * GPIO device C structure
     */
	mutable struct as_gpio_device *mDev;
};

#endif // AS_GPIO__HPP__
