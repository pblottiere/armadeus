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

#include "as_gpio.hpp"
#include <iostream>

AsGpio::AsGpio(int aGpioNum)
{
	mDev = as_gpio_open(aGpioNum);
}	

AsGpio::~AsGpio()
{
	int ret;

	if (mDev != NULL)
	{
		ret = as_gpio_close(mDev);
		if (ret < 0)
		{
			std::cout<<"AsGpio destruction error"<<std::endl;
		}
	}
	else
	{
		std::cerr<<"AsGpio device structure not allocated"<<std::endl;
	}
}

long AsGpio::setPinDirection(char *aDirection)
{
	if (mDev != NULL)
	{
		return as_gpio_set_pin_direction(mDev, aDirection);
	}
	else
	{
		std::cerr<<"AsGpio device structure not allocated"<<std::endl;
		return -1;
	}
}

const char* AsGpio::getPinDirection() const
{
	if (mDev != NULL)
	{
		return as_gpio_get_pin_direction(mDev);
	}
	else
	{
		std::cerr<<"AsGpio device structure not allocated"<<std::endl;
		return NULL;
	}
}

long AsGpio::setPinValue(int aValue)
{
	if (mDev != NULL)
	{
		return as_gpio_set_pin_value(mDev, aValue);
	}
	else
	{
		std::cerr<<"AsGpio device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsGpio::getPinValue() const
{
	if (mDev != NULL)
	{
		return as_gpio_get_pin_value(mDev);
	}
	else
	{
		std::cerr<<"AsGpio device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsGpio::waitEvent(int aDelay_ms) const
{
	if (mDev != NULL)
	{
		return as_gpio_wait_event(mDev, aDelay_ms);
	}
	else
	{
		std::cerr<<"AsGpio device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsGpio::setIrqMode(char* aMode)
{
	if (mDev != NULL)
	{
		return as_gpio_set_irq_mode(mDev, aMode);
	}
	else
	{
		std::cerr<<"AsGpio device structure not allocated"<<std::endl;
		return -1;
	}
}

const char* AsGpio::getIrqMode() const
{
	if (mDev != NULL)
	{
		return as_gpio_get_irq_mode(mDev);
	}
	else
	{
		std::cerr<<"AsGpio device structure not allocated"<<std::endl;
		return "?";
	}
}

long AsGpio::getPinNum() const
{
	if (mDev != NULL)
	{
		return as_gpio_get_pin_num(mDev);
	}
	else
	{
		std::cerr<<"AsGpio device structure not allocated"<<std::endl;
		return -1;
	}
}

