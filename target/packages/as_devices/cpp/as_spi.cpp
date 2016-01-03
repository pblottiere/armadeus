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

#include <iostream>

#include "as_spi.hpp"

#define	CHECK_INIT	\
	if (mFd <= 0)\
	{\
		std::cerr << "AsSpi class not instantianted successfully" << std::endl;\
		return -1;\
	}

AsSpi::AsSpi(const unsigned char *aSpidev_name)
{
	mFd = as_spi_open(aSpidev_name);
}

AsSpi::~AsSpi()
{
	if (mFd > 0)
	{
		as_spi_close(mFd);
	}
	else
	{
		std::cerr << "AsSpi class not instantianted successfully" << std::endl;
	}
}

int AsSpi::setLsb(unsigned char aLsb)
{
	CHECK_INIT
	return as_spi_set_lsb(mFd, aLsb);
}

int AsSpi::getLsb() const
{
	CHECK_INIT
	return as_spi_get_lsb(mFd);
}

int AsSpi::setMode(unsigned char aMode)
{
	CHECK_INIT
	return as_spi_set_mode(mFd, aMode);
}

int AsSpi::getMode() const
{
	CHECK_INIT
	return as_spi_get_mode(mFd);
}

int AsSpi::setSpeed(unsigned char aSpeed)
{
	CHECK_INIT
	return as_spi_set_speed(mFd, aSpeed);
}

int AsSpi::getSpeed() const
{
	CHECK_INIT
	return as_spi_get_speed(mFd);
}

int AsSpi::getBitsPerWord() const
{
	CHECK_INIT
	return as_spi_get_bits_per_word(mFd);
}

int AsSpi::setBitsPerWord(unsigned char aBits)
{
	CHECK_INIT
	return as_spi_set_bits_per_word(mFd, aBits);
}

unsigned long long AsSpi::msg(unsigned long long aMsg, size_t aLen, unsigned long aSpeed)
{
	CHECK_INIT
	return as_spi_msg(mFd, aMsg, aLen, aSpeed);
}
