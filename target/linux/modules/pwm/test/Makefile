ARMADEUS_BASE_DIR=../../../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

CC:=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc

OUTPUT = .

all: testtonemode testplaymode

testtonemode: testtonemode.c PWM_def.h MX1_def.h
	$(CC) -o $@ $<

testplaymode: testplaymode.c PWM_def.h MX1_def.h
	$(CC) -o $@ $<

clean:
	rm -f testtonemode testplaymode
