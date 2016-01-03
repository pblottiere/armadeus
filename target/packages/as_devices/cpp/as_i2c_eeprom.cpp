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

#include "as_i2c_eeprom.hpp"
#include <iostream>

AsI2CEeprom::AsI2CEeprom(int aBusNumber, unsigned long anEepromAddr, unsigned long anAddrWidth, \
unsigned long aPageBufferSize, unsigned int aNumberPages, unsigned int aPageSize)
{
	mDev = as_i2c_eeprom_open(aBusNumber, anEepromAddr, anAddrWidth, aPageBufferSize, aNumberPages, aPageSize);
}

AsI2CEeprom::~AsI2CEeprom()
{
	if (mDev != NULL)
	{
		as_i2c_eeprom_close(mDev);
	}
	else
	{
		std::cerr<<"AsI2CEeprom device structure not allocated"<<std::endl;
	}
}

long AsI2CEeprom::read(unsigned int aPageNumber, unsigned char* aBuffer)
{
	if (mDev != NULL)
	{
		return as_i2c_eeprom_read(mDev, aPageNumber, aBuffer);
	}
	else
	{
		std::cerr<<"AsI2CEeprom device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2CEeprom::write(unsigned int aPageNumber, unsigned char* aBuffer, unsigned int aBufferSize)
{
	if (mDev != NULL)
	{
		return as_i2c_eeprom_write(mDev, aPageNumber, aBuffer, aBufferSize);
	}
	else
	{
		std::cerr<<"AsI2CEeprom device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2CEeprom::set_write_time(unsigned int aWriteTime)
{
	if (mDev != NULL)
	{
		return as_i2c_eeprom_set_write_time(mDev, aWriteTime);
	}
	return -1;
}

long AsI2CEeprom::read_mid(unsigned int anAddress, unsigned int aLength, unsigned char* aBuffer)
{
	if (mDev != NULL)
	{
		return as_i2c_eeprom_read_mid(mDev, anAddress, aLength, aBuffer);
	}
	return -1;
}

long AsI2CEeprom::write_mid(unsigned int anAddress, unsigned int aLength, unsigned char* aBuffer)
{
	if (mDev != NULL)
	{
		return as_i2c_eeprom_write_mid(mDev, anAddress, aLength, aBuffer);
	}
	return -1;
}
