#
#  APF6 bootloader recovering tool
#
#  Copyright (C) 2014 Armadeus Project
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
# authors: sebastien.szymanski@armadeus.com
#
#

import os
import re
import serial
import subprocess
import sys
import time

from helpers import color


class UBoot(object):

    def __init__(self, serial):
        self.serial = serial

    def __getOutput(self,  pattern=r'BIOS>'):
        response = ""
        while not re.search(pattern, response):
            char = self.serial.read(1)
            if char == '':
                raise IOError
            response = response + char
        print response
        return response

    def startYModem(self):
        self.serial.write("loady\n")
        return self.__getOutput(r'C')

    def resetEnv(self,  save=True):
        if save is False:
            self.serial.write("env default -f -a\n")
        else:
            self.serial.write("run flash_reset_env\n")
        return self.__getOutput()

    def waitForPrompt(self):
        self.serial.write("\n")
        ret = self.__getOutput()
        self.serial.flushInput()
        return ret

    def flash_spl(self):
        self.serial.flushInput()
        self.serial.write("run flash_uboot_spl\n")
        return self.__getOutput()

    def flash_img(self):
        self.serial.flushInput()
        self.serial.write("run flash_uboot_img\n")
        return self.__getOutput()

    def reset(self):
        self.serial.flushInput()
        self.serial.write("reset\n")


if __name__ == "__main__":

    BR_HOST_PATH = "../../buildroot/output/host"
    BR_IMAGES_PATH = "../../buildroot/output/images"
    USB_DOWNLOAD_APP = BR_HOST_PATH + "/usr/bin/imx_usb"
    SERIAL_DOWNLOAD_APP = "sb"
    U_BOOT_SPL = "apf6-u-boot.spl"
    U_BOOT_IMG = "apf6-u-boot.img"
    SPEED = 115200

    print color.BOLD + "\n--- APF6 Bootstrap Tool ---\n"
    print "Procedure to follow:"
    print "1] Be sure to:"
    print "   * have built the APF6 BSP (make apf6_defconfig; make),"
    print "   * have \"sb\" tool installed on your PC (on Ubuntu: sudo apt-get install lrzsz),"
    print "   * have launched this script as root (sudo)."
    print "2] Power off your board, close all serial terminal sessions, remove all USB cables"
    print "3] Take jumper off BOOT0 pins and put it on BOOT1 pins."
    print "4] Power on your board"
    print "5] Connect a USB cable from your PC to the APF6Dev OTG miniUSB connector"
    print "6] Connect a USB cable from your PC to the APF6Dev console miniUSB connector"
    print ""
    port = raw_input('--- Then, enter serial port number or device name to use for console (/dev/ttyUSB0 under Linux (default) or COMx under Window$): ' + color.END)
    if port == '':
        port = '/dev/ttyUSB0'

    if not os.path.exists(USB_DOWNLOAD_APP):
        print color.RED + "Error: imx_usb is not present. Please compile Armadeus BSP first: make apf6_defconfig; make"  + color.END
        sys.exit(1)

    if not os.path.exists(U_BOOT_SPL) or not os.path.exists(U_BOOT_IMG):
        print color.RED + "Error: U-Boot binaries don't exist !" + color.END
        sys.exit(1)

    try:
        subprocess.call([SERIAL_DOWNLOAD_APP,  "--version"])
    except OSError:
        print color.RED + "Error: " + SERIAL_DOWNLOAD_APP + " not found. Did you install lrzsz package on your system ?" + color.END
        sys.exit(1)

    try:
        subprocess.check_call([USB_DOWNLOAD_APP,  "-c",  BR_HOST_PATH + "/usr/share/imx_usb_loader/",  U_BOOT_SPL])
        subprocess.check_call(SERIAL_DOWNLOAD_APP + " -k " + U_BOOT_IMG + " > " + port + " < " + port,  shell=True)
    except subprocess.CalledProcessError:
        print color.RED + "Error: Loading U-Boot binaries failed !" + color.END
        sys.exit(1)

    try:
        print "Opening serial port %s..." % port
        ser = serial.Serial(port, SPEED, timeout=5)
        ser.flush()
        uboot = UBoot(ser)
        time.sleep(1)

        uboot.waitForPrompt()

        uboot.startYModem()
        subprocess.check_call(SERIAL_DOWNLOAD_APP + " -k " + U_BOOT_SPL + " > " + port + " < " + port,  shell=True)
        uboot.waitForPrompt()
        uboot.flash_spl()

        uboot.startYModem()
        subprocess.check_call(SERIAL_DOWNLOAD_APP + " -k " + U_BOOT_IMG + " > " + port + " < " + port,  shell=True)
        uboot.waitForPrompt()
        uboot.flash_img()

        eraseAll = raw_input(color.BOLD + '\n--- Would you like to erase the environment variables ? y/N: ' + color.END)
        if eraseAll == 'y':
            uboot.resetEnv()
    except serial.SerialException:
        print "Error: Opening serial port %s failed !" % port
        sys.exit(1)
    except IOError:
        print "IOError on serial port %s" % port
        ser.close()
        sys.exit(1)
    except subprocess.CalledProcessError:
        print color.RED + "Error: Loading U-Boot binaries on serial port failed !" + color.END
        ser.close()
        sys.exit(1)
    else:
        print color.GREEN + "\n--- U-Boot successfully recovered !" + color.END
        print color.BOLD +  "--- Now you can remove the miniUSB OTG cable and put the jumper back on BOOT0 pin. Your board will boot... As the restored U-Boot version may differ from the one your were using, " + color.UNDERLINE + "if you want to maximize the chances to keep your rootfs data safe, we strongly advise you to reflash the same U-Boot version your were using before the recovery. Otherwise (you don't care about your data), please upgrade to the latest available stable Armadeus U-Boot release." + color.END
