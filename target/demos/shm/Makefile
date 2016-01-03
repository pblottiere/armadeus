# Default values for local compiling:
ARMADEUS_BASE_DIR:=$(shell pwd)/../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

CC=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
STRIP:=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)sstrip
DEFINES=-D$(ARMADEUS_BOARD_NAME)

SHMWRITER = shmwriter
SHMREADER = shmreader

default: $(SHMWRITER) $(SHMREADER)

all: $(SHMWRITER) $(SHMREADER)

$(SHMWRITER): $(SHMWRITER).c
	$(CC) $(CFLAGS) $(DEFINES) -Wall -o $@ $^ -lrt

$(SHMREADER): $(SHMREADER).c
	$(CC) $(CFLAGS) $(DEFINES) -Wall -o $@ $^ -lrt

clean: 
	rm -rf $(SHMWRITER) $(SHMREADER)
	rm -rf *.o

install: $(SHMWRITER) $(SHMREADER)
	cp $(SHMWRITER) $(INSTALL_DIR)/
	cp $(SHMREADER) $(INSTALL_DIR)/
	$(STRIP) $(INSTALL_DIR)/$(SHMWRITER)
	$(STRIP) $(INSTALL_DIR)/$(SHMREADER)

uninstall:
	rm -f $(INSTALL_DIR)/$(SHMWRITER)
	rm -f $(INSTALL_DIR)/$(SHMREADER)

.PHONY: clean install uninstall
