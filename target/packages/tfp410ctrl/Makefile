#
# Makefile for the APF51 HDMI/DVI controler (TFP410) utility
#
ARMADEUS_BASE_DIR=../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

CC = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
CFLAGS  = -Wall -O
INCLUDES = 

PWD := $(shell pwd)

SOURCES = $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)

EXE = tfp410ctrl

$(EXE): $(OBJS)
	@echo
	@echo "Building $(EXE) control program..."
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXE)

all: $(EXE)

clean:
	rm -f $(EXE)
	rm -f *.o *~ core
	rm -f parse_edid

tfp410ctrl.o: tfp410ctrl.c tfp410ctrl.h
	@echo
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

parse_edid: parse-edid.o
	@echo "Building standalone parse_edid program..."
	$(CC) $< $(LDFLAGS) -o $@
