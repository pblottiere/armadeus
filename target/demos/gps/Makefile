ARMADEUS_BASE_DIR=../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in
INSTALL_DIR:=$(ARMADEUS_ROOTFS_DIR)/usr/local/bin/
CC:=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
STRIP:=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)sstrip
CFLAGS+=-Wall -g


default: nmea_decode

all: nmea_decode

nmea_decode: nmea_decoder.c
	$(CC) $(CFLAGS) -o $@ $^

install: nmea_decode
	cp nmea_decode $(INSTALL_DIR)
	$(STRIP) $(INSTALL_DIR)/nmea_decode

clean: 
	rm -f nmea_decode
	rm -f *.o
