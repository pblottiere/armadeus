# Compil ARM 
ARMADEUS_BASE_DIR:=$(shell pwd)/../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

# Default values
INSTALL_DIR:=$(ARMADEUS_ROOTFS_DIR)/usr/bin/
INSTALL_DATA_DIR:=$(ARMADEUS_ROOTFS_DIR)/usr/share
CC=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
STRIP=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)sstrip
DEFINES=-DAPF
STAGING_DIR:=$(ARMADEUS_STAGING_DIR)
CFLAGS=$(shell STAGING_DIR=$(STAGING_DIR) sh $(STAGING_DIR)/usr/bin/sdl-config --cflags) -g
LDFLAGS=$(shell STAGING_DIR=$(STAGING_DIR) sh $(STAGING_DIR)/usr/bin/sdl-config --libs) -lSDL_ttf -lSDL_mixer

ifeq ($(TARGET),x86)
# Compil HOST
CC=gcc
STRIP=strip
CFLAGS=`/usr/bin/sdl-config --cflags`
LDFLAGS=`/usr/bin/sdl-config --libs` -lSDL_ttf -lSDL_mixer
DEFINES=-DHOST
endif


EXEC_NAME=aplayer

default: $(EXEC_NAME)

all: $(EXEC_NAME)

main.o: player.c
	$(CC) $(CFLAGS) $(DEFINES) -c -o $@ $^

$(EXEC_NAME): player.o
	$(CC) $(LDFLAGS) -o $@ $^

install: $(EXEC_NAME)
	mkdir -p $(INSTALL_DIR)
	cp -f $(EXEC_NAME) $(INSTALL_DIR)/
	$(STRIP) $(INSTALL_DIR)/$(EXEC_NAME)
	mkdir -p $(INSTALL_DATA_DIR)/$(EXEC_NAME)/data
	cp data/* $(INSTALL_DATA_DIR)/$(EXEC_NAME)/data/

uninstall:
	rm -f $(INSTALL_DIR)/$(EXEC_NAME)

clean:
	rm -rf $(EXEC_NAME)
	rm -rf *.o

.PHONY: clean install uninstall

