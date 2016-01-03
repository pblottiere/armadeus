#
#  APF27 bootloader recovering tool
#
#  Copyright (C) 2008-2009  ARMadeus Systems
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
# Change History:
#  - (Jorasse) : init DDR RAM and u-boot upload  
#

import sys
import time
from struct import *
import os
import serial
import thread
import re

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

class apf27Bootloader:

    statuscode = {  0x8D: 'data specified is out of bounds',
                    0x55: 'error during assert verification',
                    0x36: 'hash verification failed', 
                    0x33: 'certificate parsing failed or the certificate contained an unsupported key', 
                    0x35: 'signature verification failed', 
                    0x4B: 'CSF command sequence contains unsupported command identifier', 
                    0x4E: 'absence of expected CSF header',  
                    0x4D: 'CSF length is unsupported', 
                    0x2E: 'CSF TYPE does not match processor TYPE', 
                    0x2D: 'CSF UID does not match either processor UID or generic UID', 
                    0x3A: 'CSF customer/product code does not match processor customer/product code',                    
                    0x87: 'key indexis either unsupported, or an attempt is made to overwrite the SRK from a CSF command', 
                    0xF0: 'successful operation completion', 
                    0x17: 'SCC unexpectedly not in secure state', 
                    0x1E: 'secureRAM secret key invalid', 
                    0x1D: 'secureRAM initialization failure', 
                    0x1B: 'secureRAM self test failure', 
                    0x2B: 'secureRAM internal failure', 
                    0x27: 'secureRAM secrect key unexpectedly in use', 
                    0x8B: 'an attempt is made to read a key from the list of subordinate public keys at a location where no key is installed', 
                    0x8E: 'algorithm type is either invalid or ortherwise unsupported', 
                    0x66: 'write operation to register failed', 
                    0x63: 'DCD invalid', 
                    0x6F: 'RAM application pointer is NULL or ERASED_FLASH', 
                    0x69: 'CSF missing when HAB TYPE is not HAB-disabled', 
                    0x6A: 'NANDFC boot buffer load failed', 
                    0x6C: 'Exception has occured', 
                    0x67: 'INT_BOOT fuse is blown but BOOT pins are set for external boot',
                    0x88: 'Successful download completion' }



    def __init__(self, serialport ):
        self.port = serialport


    def getstatus(self):
        self.port.flushInput()
        #print 'get Status'
        self.port.write( self.buildcmd("0505" ,"00000000","b","00000000","00","0" ))
        result = self.port.read(4)
        if len(result) > 0:
            try:
                print self.statuscode[ord(result[0])]
                #for i in range(len(result)):
                #    print '%02x' % ord(result[i]),
            except KeyError,e:
                print "Unknown status "+str(ord(result[0]))
        else:
            print '\nread timeout' 

    def __displayProgress(self, text):
        sys.stdout.write(chr(13))
        print text,
        sys.stdout.flush()

# addr: address to read
# count: number of bytes to read 
# mode: b, w, l for byte, word, long

    def get( self, addr, count, mode ):
        assert len(addr) <= 8, 'address too long'
        assert int(count,16) > 0, 'zero length get'
        assert int(count,16) < 0xffffffff, 'Too long get, should be < 31 bytes'   
        self.port.write( self.buildcmd("0101" ,addr,mode,count.zfill(9-len(count)),"00000000","0" ) )

        #nbbyte = 1
        #if mode == 'w':
        #    nbbyte = 2
        #if mode == 'l':
        #    nbbyte = 4

        #nbbyte = nbbyte * int(count)
        temp = self.port.read(int(count)+4) # read n+4 char
        if len(temp) == 0:
            print 'read timeout'
            return
        #print '\nread ACK:',
        #for i in range (len(temp)):
        #    print '%02x' % ord(temp[i]),

        #print 'read data:',
        result = []
        for i in range (4,len(temp)):
            
            result += '%02x' % ord(temp[len(temp)-i+3]),
        #print ''.join(result)
        return ''.join(result)


# addr: address where the data have to be written
# data: data to write. must match the given mode

# mode: b, w, l for byte, word, long

    def put( self, addr, data, mode ):
        assert len(data) > 0, 'zero length put'
        assert len(addr) == 8, 'address too long'
        self.port.write( self.buildcmd("0202" ,addr,mode,"00000000",data,"0" ))
        result = self.port.read(4) #ack
        if len(result) == 0: 
            print 'read timeout'
            return
        #print ' \nwrite ACK:',
        #for i in range (len(result)):
        #    print '%02x' % ord(result[i]),

        result = self.port.read(4)
        if len(result) == 0:
            print 'read timeout'
            return

        #print '\nwrite success:',
        #for i in range (len(result)):
            #print '%02x' % ord(result[i]),
        

    def download( self, addr, data, size):
        assert size > 0, 'file empty'
        f=open(data, 'rb')
        self.port.write( self.buildcmd("0404" ,addr,"l", size, "00000000","AA" ))
        result = self.port.read(4) #ack
        if len(result) == 0: 
            print '\nread timeout'
            return
        #print ' \nwrite ACK:',
        #for i in range (len(result)):
        #    print '%02x' % ord(result[i]),

        data = f.read(16)
        count = 16
        b = 0
        print '\nTransfering:'
        while len(data):
            n = len(data)
            b = 16-n  # ensure always 4 bytes
            #tr = unpack('B'*n,data)
            self.port.write(data)
            count = count + 16
            if (count % 1024) == 0:
                self.__displayProgress("%06d bytes" % count)
            data = f.read(16)
            time.sleep(0.001)
        self.__displayProgress("%d bytes transfered" % count)
        print ""
        f.close()
        self.getstatus()
        return

    def buildcmd( self, cmd, addr, ds, cnt, data, ft ):
        if ds == 'b':
            datatmp = ('%c' % int('00',16)) + ('%c' % int('00',16)) + ('%c' % int('00',16)) + ('%c' % int(data[0:2],16))
            datasize = "08"
        elif ds == 'w':
            datasize = "10"
            datatmp = ('%c' % int('00',16)) + ('%c' % int('00',16)) + ('%c' % int(data[0:2],16))  + ('%c' % int(data[2:4],16))
        elif ds == 'l':
            datasize = "20"
            datatmp  = ('%c' % int(data[0:2],16)) + ('%c' % int(data[2:4],16)) + ('%c' % int(data[4:6],16))  + ('%c' % int(data[6:8],16))
        else:
            print "error invalid data size"
            return ""

        result = ('%c' % int(cmd[0:2],16)) + ('%c' % int(cmd[2:4],16)) + \
                 ('%c' % int(addr[0:2],16)) + ('%c' % int(addr[2:4],16)) + ('%c' % int(addr[4:6],16)) + ('%c' % int(addr[6:8],16)) + \
                 ('%c' % int(datasize,16)) + \
                 ('%c' % int(cnt[0:2],16)) + ('%c' % int(cnt[2:4],16)) + ('%c' % int(cnt[4:6],16)) + ('%c' % int(cnt[6:8],16)) + \
                 datatmp + ('%c' % int(ft,16))

        #for i in range(len(result)):
        #    print '%02x' % ord(result[i]),
        #print cnt
        return result   

    def initSDRAM2( self ):
        print "SDRAM INIT2 APF",
        self.put("D800100C","0069D42A","l")

        self.put("D8001008","92126485","l")
        self.put("B0000400","00000000","l")
        self.put("D8001008","A2126485","l")
        self.put("B0000000","00000000","l")
        self.put("B0000000","00000000","l")
        self.put("B0000000","00000000","l")
        self.put("B0000000","00000000","l")
        self.put("D8001008","B2126485","l")
        self.put("B0000033","DA0000DA","b")
        self.put("B2000000","00000000","b")
        self.put("D8001008","82126485","l")

    def initSDRAM1( self ):

        print "SDRAM INIT1 APF",
        self.put("10000000","20040304","l") #/* AIPI1_PSR0 */
        self.put("10000004","DFFBFCFB","l") #/* AIPI1_PSR1 */
        self.put("10020000","00000000","l") #/* AIPI2_PSR0 */
        self.put("10020004","FFFFFFFF","l") #/* AIPI2_PSR1 */
        self.put("10027818","0003000F","l") #/* GPCR */

        self.put("D8002060","00002200","l") #/* EIM / WCR */

        self.put("D8001010","0000002C","l")

        self.put("D8001004","0069D42A","l")

        self.put("D8001000","92126485","l")
        self.put("A0000400","00000000","l")
        self.put("D8001000","A2126485","l")
        self.put("A0000000","00000000","l")
        self.put("A0000000","00000000","l")
        self.put("A0000000","00000000","l")
        self.put("A0000000","00000000","l")
        self.put("D8001000","B2126485","l")
        self.put("A0000033","DA0000DA","b")
        self.put("A2000000","00000000","b")
        self.put("D8001000","82126485","l")


    def consoleDump(tid):
        while 1:
            sys.stdout.write(tid.read(1))
    #        print '%s' % tid.read(1),

class UBoot:
    """
    """
    def __init__(self, bootstrap, serial, binary):
        self.bootstrap = bootstrap
        self.serial = serial
        self.binary = binary

    def __getOutput(self,error_msg=None):
        """ This function waits for U-Boot response and return response"""
        #debugser = serial.Serial("/dev/tty0",115200,timeout=1)
        response = ""
        while not re.search(r'BIOS>',response):
            char = self.serial.read(1)
            if char == '':
                raise Error(response)
            response = response + char
            if error_msg!=None:
                if re.search(error_msg,response):
                    raise Error("getOutput error")
            sys.stdout.write(char)
            #debugser.write(char)
        #debugser.close()
        return response
 
    def load(self):
        if not os.path.exists(self.binary):
            raise Error("file "+self.binary+" doesn't exit",0)
        self.fsize = "%08x" % os.path.getsize(self.binary) #os.stat(filesize)[6]
        self.bootstrap.download("A0000000", self.binary, self.fsize)

    def resetenv(self):
        self.serial.flushInput()
        self.serial.write("env default -f && saveenv && echo Flash environment variables erased!\n")
        return self.__getOutput()

    def waitForPrompt(self):
        self.serial.write("\003")
        while not self.serial.inWaiting():
            time.sleep(0.001)
            self.serial.write("\003")
        return self.__getOutput()

    def flash(self):
        """ flash uboot """
        self.serial.flushInput()
        self.serial.write("setenv fileaddr A0000000; setenv filesize "+self.fsize+";run flash_uboot\n")
        try:
            ret = self.__getOutput()
        except Error,e:
            raise Error("Flashing U-Boot")
        if not re.search(r'uboot succeed',ret):
            print ret
            raise Error("U-Boot flash failed",0)
        return ret
    

if __name__ == "__main__":    

    RECOVERY_URL = "http://sourceforge.net/projects/armadeus/files/armadeus/"
    RECOVERY_VERSION = "armadeus-5.2"
    RECOVERY_BOARD_PATH = "binaries/apf27/"
    UBOOT_IMAGE_BIN = "apf27-u-boot-nand.bin"
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

    SPEED = 115200

    print "\nAPF27 Bootstrap Tool\n"
    print "/!\\ Do not forget to put the bootstrap jumper and to reset your board /!\\"
    print "(be sure to have a file named apf27-u-boot-nand.bin in current dir too)\n"
    print("/!\\ U-Boot recover eat lot of CPU, be sure that no process trust your CPU\n")
    port = raw_input('Enter serial port number or name to use (/dev/ttySx under Linux and COMx under Windows): ')
    try:
        ser = serial.Serial(port, SPEED, timeout=3) 
    except Exception, msg:
        print "unable to open serial port %s" % port
        print msg
        sys.exit()
    ser.flush()

    apfBootloader = apf27Bootloader(ser)
    uboot = UBoot(apfBootloader, ser, UBOOT_IMAGE_BIN)

    apfBootloader.getstatus()
    apfBootloader.initSDRAM1()
    apfBootloader.initSDRAM2()
    uboot.load()
    uboot.waitForPrompt()
    eraseAll = raw_input('Would you like to erase the environment variables? y/n: ')
    if eraseAll == 'y':
        uboot.resetenv()
    uboot.flash()
    print "U-Boot successfully recovered !"

    del ser

