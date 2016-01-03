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

#include "as_pwm.hpp"
#include <iostream>

AsPwm::AsPwm(int aPwmNumber)
{
	mDev = as_pwm_open(aPwmNumber);
}	

AsPwm::~AsPwm()
{
	int ret;

	if (mDev != NULL)
	{
		ret = as_pwm_close(mDev);
		if (ret < 0)
		{
			std::cout<<"AsPwm destruction error"<<std::endl;
		}
	}
	else
	{
		std::cerr<<"AsPwm device structure not allocated"<<std::endl;
	}
}

long AsPwm::setFrequency(int aFrequency)
{
	if (mDev != NULL)
	{
		return as_pwm_set_frequency(mDev, aFrequency);
	}
	else
	{
		std::cerr<<"AsPwm device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsPwm::getFrequency() const
{
	if (mDev != NULL)
	{
		return as_pwm_get_frequency(mDev);
	}
	else
	{
		std::cerr<<"AsPwm device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsPwm::setPeriod(int aPeriod)
{
	if (mDev != NULL)
	{
		return as_pwm_set_period(mDev, aPeriod);
	}
	else
	{
		std::cerr<<"AsPwm device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsPwm::getPeriod() const
{
	if (mDev != NULL)
	{
		return as_pwm_get_period(mDev);
	}
	else
	{
		std::cerr<<"AsPwm device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsPwm::setDuty(int aDuty)
{
	if (mDev != NULL)
	{
		return as_pwm_set_duty(mDev, aDuty);
	}
	else
	{
		std::cerr<<"AsPwm device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsPwm::getDuty() const
{
	if (mDev != NULL)
	{
		return as_pwm_get_duty(mDev);
	}
	else
	{
		std::cerr<<"AsPwm device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsPwm::setState(int aEnable)
{
	if (mDev != NULL)
	{
		return as_pwm_set_state(mDev, aEnable);
	}
	else
	{
		std::cerr<<"AsPwm device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsPwm::getState() const
{
	if (mDev != NULL)
	{
		return as_pwm_get_state(mDev);
	}
	else
	{
		std::cerr<<"AsPwm device structure not allocated"<<std::endl;
		return -1;
	}
}
