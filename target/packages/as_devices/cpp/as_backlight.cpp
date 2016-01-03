/*
**    THE ARMadeus Systems
**
**    Copyright (C) 2012  The armadeus systems team
**    J�r�mie Scheer <jeremie.scheeer@armadeus.com>
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

#include "as_backlight.hpp"
#include <iostream>

AsBacklight::AsBacklight(char *aBacklightName)
{
	mDev = as_backlight_open2(aBacklightName);
}

AsBacklight::AsBacklight()
{
	mDev = as_backlight_open();
}

AsBacklight::~AsBacklight()
{
	int ret;

	if (mDev != NULL)
	{
		ret = as_backlight_close(mDev);
		if (ret < 0)
		{
			std::cout<<"AsBacklight destruction error"<<std::endl;
		}
	}
	else
	{
		std::cerr<<"AsBacklight device structure not allocated"<<std::endl;
	}
}

long AsBacklight::getActualBrightness() const
{
	if (mDev != NULL)
	{
		return as_backlight_get_actual_brightness(mDev);
	}
	else
	{
		std::cerr<<"AsBacklight device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsBacklight::getMaxBrightness() const
{
	if (mDev != NULL)
	{
		return as_backlight_get_max_brightness(mDev);
	}
	else
	{
		std::cerr<<"AsBacklight device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsBacklight::setBrightness(int aBrightness)
{
	if (mDev != NULL)
	{
		return as_backlight_set_brightness(mDev, aBrightness);
	}
	else
	{
		std::cerr<<"AsBacklight device structure not allocated"<<std::endl;
		return -1;
	}
}
