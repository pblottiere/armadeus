ARMADEUS_BASE_DIR=../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

CC:=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
CFLAGS = -Wall
INCLUDE = -I$(ARMADEUS_STAGING_DIR)/usr/include/as_devices/
INSTALL_DIR = $(ARMADEUS_ROOTFS_DIR)/usr/bin/

EXEC = toggle_gpio

$(EXEC): toggle_gpio.c
	$(CC) $(CFLAGS) toggle_gpio.c -o $(EXEC) -las_devices $(INCLUDE)

install: $(EXEC)
	cp -f $(EXEC) $(INSTALL_DIR)

clean:
	rm -f *.o $(EXEC)

.PHONY: install clean
