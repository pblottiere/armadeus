#
#  APF28 bootloader recovering tool
#
#  Copyright (C) 2012 Armadeus Project
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
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
# authors: eric.jarrige@armadeus.org
#  
#

import sys
import time
from struct import *
import os
import serial
import thread
import re

import usb

class Error(Exception):
    """ Manage specific error

    attributes:
        message -- the message
        level   -- the exception level
    """

    def __init__(self,message,level=0):
        self._message = message
        self.level   = level
        Exception.__init__(self,message)
                
    def __repr__(self):
        return self.message

    def _get_message(self): 
        return self._message
    def _set_message(self, message): 
        self._message = message
    message = property(_get_message, _set_message)


    def __str__(self):
        print self.message

    def setLevel(self,level):
        self.level = int(str(level))
    def getLevel(self):
        return self.level




class UBoot:
    """
    """
    def __init__(self, serial):
        self.serial = serial

    def __getOutput(self, error_msg=None):
        """ This function waits for U-Boot answer and returns it"""
        response = ""
        while not re.search(r'BIOS>', response):
            char = self.serial.read(1)
            if char == '':
                print "serial timeout"
                raise response
            response = response + char
            if error_msg!=None:
                if re.search(error_msg,response):
                    raise Error("getOutput error")
        sys.stdout.write(response)
        return response
 
    def resetEnv(self):
        self.serial.write("run flash_reset_env\n")
        return self.__getOutput()

    def importEnv(self, addr, size):
        self.serial.write("env import -t %08x %08x\n" % (addr, size))
        return self.__getOutput()

    def waitForPrompt(self):
        time.sleep(1)
        self.serial.write("\n")
        ret = self.__getOutput()
        self.serial.flushInput()
        return ret

    def flash(self):
        """ Ask U-Boot to flash itself """
        time.sleep(1)
        self.serial.flushInput()
        self.serial.write("run flash_uboot\n")
        try:
            ret = self.__getOutput()
            print ret
        except Error,e:
            raise Error("Flashing U-Boot")
        if not re.search(r'uboot succeed',ret):
            print ret
            raise Error("U-Boot flash failed",0)
        return ret

    def reset(self):
        """ Ask U-Boot to flash itself """
        time.sleep(1)
        self.serial.flushInput()
        self.serial.write("reset\n")
        try:
            ret = self.__getOutput()
            print ret
        except Error,e:
            raise Error("Reseting U-Boot")
        return ret
    

if __name__ == "__main__":

    BR_HOST_PATH = "../../buildroot/output/host"
    SPEED = 115200
    BINARY = "apf28-u-boot.sb"
    USB_DOWNLOAD_APP = BR_HOST_PATH + "/usr/bin/gnet-imx28-usb-downloader"
    ELFTOSB_APP = BR_HOST_PATH + "/usr/bin/elftosb"
    NAND_BINARY = "apf28_nand_recover.sb"
    ENV_RECOVER = "apf28_env_recover.txt"
    ENV_DOWNLOAD = 0x42000000

    print "\n--- APF28 Bootstrap Tool ---\n"
    print "Procedure to follow:"
    print "1] Power off your board, close all serial terminal sessions, remove all USB cables"
    print "2] Put bootstrap jumper"
    print "3] Power on your board"
    print "4] Connect a USB cable from your PC to the APF28Dev OTG miniUSB connector"
    print "5] Connect a USB cable from your PC to the APF28Dev console miniUSB connector"
    print "6] Be sure to have built the APF28 BSP: make apf28_defconfig; make"
    print "7] Have launched this script as root (sudo)\n"
    port = raw_input('--- Enter serial port number or device name to use for console (/dev/ttyACM0 under Linux (default) or COMx under Window$): ')
    if port is '':
        port = '/dev/ttyACM0'

    if not os.path.exists(USB_DOWNLOAD_APP):
        raise Error("gnet-imx28-usb-downloader is not present: please compile Armadeus first ",0)

    if not os.path.exists(ELFTOSB_APP):
        raise Error("elftosb is not present: please compile Armadeus first ",0)

    if not os.path.exists(BINARY):
        raise Error("file "+BINARY+" doesn't exit",0)

    print "Opening serial port %s.." % port
    try:
        ser = serial.Serial(port, SPEED, timeout=4)
    except Exception, msg:
        print "Unable to open serial port %s !" % port
        print msg
        sys.exit()
    ser.flush()

    uboot = UBoot(ser)

    print "Building NAND image file %s.." % NAND_BINARY

    fsize = "%08x" % os.path.getsize(BINARY)
    envfile = open(ENV_RECOVER, 'w')
    envfile.write("fileaddr=41000000\n")
    envfile.write("filesize=%s\n" % fsize)
    envfile.close()
    
 
    try:
        os.system(ELFTOSB_APP + " -zdf imx28 -c apf28-u-boot_recover.bd -o " + NAND_BINARY)
    except Exception, msg:
        print "Failed to build build NAND image filet !"
        print msg
        os.remove(NAND_BINARY);
        os.remove(ENV_RECOVER);
        sys.exit()

    print "Downloading NAND image file %s.." % NAND_BINARY
    try:
        os.system(USB_DOWNLOAD_APP + " -f " + NAND_BINARY)
    except Exception, msg:
        print "Failed to upload NAND image file to the target !"
        print msg
        os.remove(NAND_BINARY);
        os.remove(ENV_RECOVER);
        sys.exit()

    os.remove(NAND_BINARY);

    print "--- Successfully initialized APF28, now loading U-Boot"
    
    print "Loading %s, size = %d bytes.." % (BINARY, os.path.getsize(BINARY))
    time.sleep(4)
   
    try:
        os.system(USB_DOWNLOAD_APP + " -f " + BINARY)
    except Exception, msg:
        print "Failed to upload image file to the target !"
        print msg
        sys.exit()

    uboot.waitForPrompt()

    eraseAll = raw_input('\n--- Would you like to erase the environment variables ? y/N: ')
    if eraseAll == 'y':
        uboot.resetEnv()

    uboot.importEnv(ENV_DOWNLOAD, os.path.getsize(ENV_RECOVER))

    os.remove(ENV_RECOVER);

    print "Programming U-Boot in NAND flash.."
    uboot.flash()

    print "\n--- U-Boot successfully recovered !"
    print "--- Now you can remove miniUSB OTG cable and bootstrap jumper. Your board is running..."

    del ser

