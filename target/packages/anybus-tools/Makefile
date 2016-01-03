# Default values for local compiling:
ARMADEUS_BASE_DIR=../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

STAGING_DIR:=$(ARMADEUS_BUILD_DIR)/staging_dir/
INSTALL_DIR:=$(ARMADEUS_ROOTFS_DIR)/usr/bin/
INITD_DIR:=$(ARMADEUS_ROOTFS_DIR)/etc/init.d/
CC:=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
STRIP:=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)sstrip

DEFINES=TARGET
CFLAGS= -W -Wall -g
LIB= -lncurses

EXEC=anybus-tools

default: $(EXEC)

all: $(EXEC)

read_reg.o: read_reg.c
	$(CC) $(CFLAGS) -c -o $@ $^

about_help.o: about_help.c
	$(CC) $(CFLAGS) -c -o $@ $^

mailbox.o: mailbox.c
	$(CC) $(CFLAGS) -c -o $@ $^

edit_input.o: edit_input.c
	$(CC) $(CFLAGS) -c -o $@ $^

map_output.o: map_output.c
	$(CC) $(CFLAGS) -c -o $@ $^

sel_iface.o: sel_iface.c
	$(CC) $(CFLAGS) -c -o $@ $^

win.o: win.c
	$(CC) $(CFLAGS) -c -o $@ $^

anybus-tools.o: anybus-tools.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(EXEC): read_reg.o about_help.o mailbox.o edit_input.o map_output.o sel_iface.o win.o anybus-tools.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIB)

clean:
	rm -rf anybus-tools
	rm -rf *.o

.PHONY: clean install uninstall

