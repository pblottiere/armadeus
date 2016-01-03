#! /usr/bin/python
# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------
# Name:     AsSpi.py
# Purpose:  
# Author:   Fabien Marteau <fabien.marteau@armadeus.com>
# Created:  20/07/2012
#-----------------------------------------------------------------------------
#  Copyright (2012)  Armadeus Systems
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software 
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
#-----------------------------------------------------------------------------

## @file
# @brief Provide Python wrapper for SPI
# 
# @ingroup python_wrappers
#

__doc__ = "This class drive ARMadeus Board SPI"
__version__ = "0.1"
__author__ = "Fabien Marteau <fabien.marteau@armadeus.com>"

import wrappers.AsSpi_wrap as wrapper

##
#    @brief Python wrapper for SPI
#
#    @ingroup python_wrappers
#
class AsSpiError(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

class AsSpi:
    """
    """

    def __init__(self, aSpidevName):
        try:
            self.__device = wrapper.spi_open(aSpidevName)
        except Exception, e:
            raise AsSpiError(str(e))

    def __del__(self):
        try:
            wrapper.spi_close(self.__device)
        except Exception, e:
            pass

    def setLsb(self, aLsb):
        """ Set lsb
        """
        try:
            return wrapper.spi_set_lsb(self.__device, aLsb)
        except Exception, e:
            raise AsSpiError(str(e))

    def getLsb(self):
        """ Get lsb
        """
        try:
            return wrapper.spi_get_lsb(self.__device)
        except Exception, e:
            raise AsSpiError(str(e))

    def setMode(self, aMode):
        """ Set mode
        """
        try:
            return wrapper.spi_set_mode(self.__device, aMode)
        except Exception, e:
            raise AsSpiError(str(e))

    def getMode(self):
        """ Set mode
        """
        try:
            return wrapper.spi_get_mode(self.__device)
        except Exception, e:
            raise AsSpiError(str(e))

    def setSpeed(self, aSpeed):
        """ Set speed
        """
        try:
            return wrapper.spi_set_speed(self.__device, aSpeed)
        except Exception, e:
            raise AsSpiError(str(e))

    def getSpeed(self):
        """ get speed
        """
        try:
            return wrapper.spi_get_speed(self.__device)
        except Exception, e:
            raise AsSpiError(str(e))

    def getBitsPerWord(self):
        """ Get bits per word
        """
        try:
            return wrapper.spi_get_bits_per_word(self.__device)
        except Exception, e:
            raise AsSpiError(str(e))

    def setBitsPerWord(self, aBits):
        """ Set bits per word
        """
        try:
            return wrapper.spi_set_bits_per_word(self.__device, aBits)
        except Exception, e:
            raise AsSpiError(str(e))

    def msg(self, aSpeed, aLen, aMsg):
        """ send/rcv message
            aSpeed : spi speed
            aLen : number of bits to send
            aMsg : list of bytes to send (max 8)
        """
        if aLen <= 0:
            raise AsSpiError("aLen must be upper than 0")
        if aLen > 64:
            raise AsSpiError("aLen can't be upper than 64")

        if type(aMsg) != type([]):
            raise AsSpiError("aMsg argument must be a list")
        if len(aMsg) <= 0:
            raise AsSpiError("aMsg list must have at least one value")
        if len(aMsg) > 8:
            raise AsSpiError("aMsg list can have 8 values maximum")
   
        if aLen%8 == 0:
            list_len = aLen/8
        else:
            list_len = (aLen/8) + 1

        msg = aMsg + [0 for i in range(8 - list_len)]

        try:
            return wrapper.spi_msg(self.__device, aSpeed, aLen,
                    msg[7], msg[6], msg[5], msg[4], msg[3], msg[2], msg[1], msg[0])
        except Exception, e:
            raise AsSpiError(str(e))

