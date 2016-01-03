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

#include "as_93lcxx.hpp"
#include <iostream>

As93LCXX::As93LCXX(unsigned char *aSpidevFilename, unsigned char aType, unsigned long aSpeed, unsigned char aWordSize)
{
	mDev = as_93lcxx_open(aSpidevFilename, aType, aSpeed, aWordSize);
}

As93LCXX::~As93LCXX()
{
	if (mDev != NULL)
	{
		as_93lcxx_close(mDev);
	}
	else
	{
		std::cerr<<"As93LCXX device structure not allocated"<<std::endl;
	}
}

long As93LCXX::read(unsigned int anAddress) const
{
	if (mDev != NULL)
	{
		return as_93lcxx_read(mDev, anAddress);
	}
	else
	{
		std::cerr<<"As93LCXX device structure not allocated"<<std::endl;
		return -1;
	}
}

long As93LCXX::ewen()
{
	if (mDev != NULL)
	{
		return as_93lcxx_ewen(mDev);
	}
	else
	{
		std::cerr<<"As93LCXX device structure not allocated"<<std::endl;
		return -1;
	}
}

long As93LCXX::erase(unsigned int anAddress)
{
	if (mDev != NULL)
	{
		return as_93lcxx_erase(mDev, anAddress);
	}
	else
	{
		std::cerr<<"As93LCXX device structure not allocated"<<std::endl;
		return -1;
	}
}

long As93LCXX::eraseAll()
{
	if (mDev != NULL)
	{
		return as_93lcxx_erase_all(mDev);
	}
	else
	{
		std::cerr<<"As93LCXX device structure not allocated"<<std::endl;
		return -1;
	}
}

long As93LCXX::write(unsigned int anAddress, unsigned int aValue)
{
	if (mDev != NULL)
	{
		return as_93lcxx_write(mDev, anAddress, aValue);
	}
	else
	{
		std::cerr<<"As93LCXX device structure not allocated"<<std::endl;
		return -1;
	}
}

long As93LCXX::writeAll(unsigned int aValue)
{
	if (mDev != NULL)
	{
		return as_93lcxx_write_all(mDev, aValue);
	}
	else
	{
		std::cerr<<"As93LCXX device structure not allocated"<<std::endl;
		return -1;
	}
}

long As93LCXX::ewds()
{
	if (mDev != NULL)
	{
		return as_93lcxx_ewds(mDev);
	}
	else
	{
		std::cerr<<"As93LCXX device structure not allocated"<<std::endl;
		return -1;
	}
}
