#
#  APF9328 boot revovery tool
#
#  Copyright (2006)  ARMADEUS
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
# author: nicolas.colombain@armadeus.com
#

from bootloader import *
import sys
import serial

ser = 0 #serial port

if __name__ == "__main__":    
    RECOVERY_URL = "http://sourceforge.net/projects/armadeus/files/armadeus/"
    RECOVERY_VERSION = "armadeus-5.2"
    RECOVERY_BOARD_PATH = "binaries/apf9328/"
    UBOOT_IMAGE_BIN = "apf9328-u-boot.bin"
    UBOOT_RECOVERY_PATH = RECOVERY_URL+RECOVERY_VERSION+"/"+RECOVERY_BOARD_PATH+UBOOT_IMAGE_BIN+"/download"
    if not os.path.exists(UBOOT_IMAGE_BIN):
        try:
            print "Downloading " + UBOOT_IMAGE_BIN + ".."
            os.system("wget -O "+ UBOOT_IMAGE_BIN + " " +UBOOT_RECOVERY_PATH)
        except Exception, msg:
            print "Failed to download U-Boot image file from server!"
            print msg
            print "Server may be unavailable for the momenet. please try later."
            os.remove(UBOOT_IMAGE_BIN);
            sys.exit()

    SPEED = 38400
    ramSize = 16
    print "APF Recover Tool"
    print "Do not forget to place the bootstrap jumper !!"
    print "and be sure to have a file named apf9328-u-boot.bin in current dir"
    port = raw_input('Enter serial port number or name to use (/dev/ttySx under Linux and COMx under Windows: ')
    input = raw_input('Enter RAM size (default 16): ')
    if input.isdigit():
        ramSize = int( input )
    try:
        ser = serial.Serial(port, SPEED, timeout=1) 
    except Exception, msg:
        print "unable to open serial port %s" % port
	print msg
        sys.exit()
    ser.flush()
    apfBootloader = imxlBootloader(ser, ramSize)
    apfBootloader.sync()
    eraseAll = raw_input('Would you like to erase the environment variables? y/n: ')
    apfBootloader.loadFlashbin(UBOOT_IMAGE_BIN, "10000000", eraseAll)
    print "uBoot successfully recovered !"
    del ser

