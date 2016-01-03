ARMADEUS_BASE_DIR=../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

CC:=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
CFLAGS=-g -W -Wall
LDFLAGS=-lm
INSTALL_DIR=/tftpboot


SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
EXEC=oscillo

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean install

clean:
	rm -rf *.o
	rm -f $(EXEC)

install: all
	cp -f $(EXEC) $(INSTALL_DIR)

