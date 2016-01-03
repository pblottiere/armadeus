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

#include "as_i2c.hpp"
#include <iostream>


AsI2c::AsI2c(int aBusNumber)
{
	mDev = as_i2c_open(aBusNumber);
}

AsI2c::~AsI2c()
{
	int ret;

	if (mDev != NULL)
	{
		ret = as_i2c_close(mDev);
		if (ret < 0)
		{
			std::cout<<"AsI2c destruction error"<<std::endl;
		}
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
	}
}

long AsI2c::setSlaveAddr(unsigned char aAddr)
{
	if (mDev != NULL)
	{
		return as_i2c_set_slave_addr(mDev, aAddr);
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2c::getSlaveAddr() const
{
	if (mDev != NULL)
	{
		return as_i2c_get_slave_addr(mDev);
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2c::read(unsigned char *aData, size_t aSize) const
{
	if (mDev != NULL)
	{
		return as_i2c_read(mDev, aData, aSize);
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2c::write(unsigned char *aData, size_t aSize)
{
	if (mDev != NULL)
	{
		return as_i2c_write(mDev, aData, aSize);
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2c::readReg(unsigned char aReg, unsigned char *aData, size_t aSize) const
{
	if (mDev != NULL)
	{
		return as_i2c_read_reg(mDev, aReg, aData, aSize);
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2c::writeReg(unsigned char aReg, unsigned char *aData, size_t aSize)
{
	if (mDev != NULL)
	{
		return as_i2c_write_reg(mDev, aReg, aData, aSize);
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2c::readMsg(unsigned char *aWData, unsigned char aWriteSize, unsigned char *aRData, size_t aReadSize)
{
	if (mDev != NULL)
	{
		return as_i2c_read_msg(mDev, aWData, aWriteSize, aRData, aReadSize);
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2c::readRegByte(unsigned char aReg) const
{
	if (mDev != NULL)
	{
		return as_i2c_read_reg_byte(mDev, aReg);
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
		return -1;
	}
}

long AsI2c::writeRegByte(unsigned char aReg, unsigned char aVal)
{
	if (mDev != NULL)
	{
		return as_i2c_write_reg_byte(mDev, aReg, aVal);
	}
	else
	{
		std::cerr<<"AsI2c device structure not allocated"<<std::endl;
		return -1;
	}
}	
