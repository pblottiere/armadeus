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
 * @brief Provide C++ wrapper for PWM device
 *
 * @ingroup cpp_wrappers
 */

#ifndef AS_PWM__HPP__
#define AS_PWM__HPP__

#include "as_pwm.h"

/**
  * C++ wrapper for PWM device
  *
  * @ingroup cpp_wrappers
  */
class AsPwm
{
public:
        /**
          * Constructor
          *
          * @param aPwmNumber   PWM device number
          */
	AsPwm(int aPwmNumber);
	virtual ~AsPwm();

	/** @brief set frequency
	 *
	 * @param aFrequency frequency in Hz
	 *
	 * @return negative value on error
	 */
	long setFrequency(int aFrequency);

        /** @brief get frequency
	 *
	 * @return frequency value in Hz, negative value on error
	 */
	long getFrequency() const;

	/** @brief set period
	 *
	 * @param aPeriod period in uS
	 *
	 * @return negative value on error
	 */
	long setPeriod(int aPeriod);

	/** @brief get period value
	 *
	 * @return period value in uS, negative value on error.
	 */
	long getPeriod() const;
	
	/** @brief set duty
	 *
	 * @param aDuty duty in 0.1%
	 *
	 * @return negative value on error
	 */
	long setDuty(int aDuty);

	/** @brief get duty
	 *
	 * @return duty value in 0.1%, negative value on error
	 */
	long getDuty() const;

	/** @brief set state of pwm
	 *
	 * @param aEnable 0:disable, 1:enable
	 *
	 * @return negative value on error
	 */
	long setState(int aEnable);

	/** @brief get state of pwm
	 *
	 * @return 0:disabled, 1:enabled, negative value on error
	 */
	long getState() const;

protected:
    /**
     * PWM device C structure
     */
	mutable struct as_pwm_device *mDev;
};

#endif // AS_PWM__HPP__
