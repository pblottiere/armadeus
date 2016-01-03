# Default values for local compiling:
ARMADEUS_BASE_DIR = ../../../../
-include $(ARMADEUS_BASE_DIR)/Makefile.in
CFLAGS ?= -Wall -g -O0
LDFLAGS =
CC = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc

OBJ += as_pwm.o
OBJ += as_i2c.o
ifeq ($(ARMADEUS_LINUX_VERSION),2.6.29.6)
OBJ += as_gpio_2_6_29.o
else
OBJ += as_gpio.o
OBJ += as_led.o
endif
OBJ += as_dac.o
OBJ += as_adc.o

OBJ += as_adc_iio.o
OBJ += as_93lcxx.o
OBJ += as_spi.o
OBJ += as_as1531.o
OBJ += as_max1027.o
OBJ += as_max5821.o
OBJ += as_backlight.o
OBJ += as_i2c_eeprom.o
OBJ += as_helpers.o
OBJ += as_power_supply.o
OBJ += as_tcs3472.o

LIBRARY = as_devices
LIB_VERS = 1
STAT_LIB = $(LIBRARY).a
SONAME = lib$(LIBRARY).so
LIBNAME = $(SONAME).$(LIB_VERS)
ARCHIVE_CMD = ar
ARCHIVE_OPTS = -cvq

$(LIBNAME): $(OBJ)
	@$(CC) -shared -Wl,-soname,$(SONAME) -o $(SONAME) $(OBJ)
	@ln -sf $(SONAME) $(LIBNAME)
	@echo "  LNK $@"

$(STAT_LIB): $(OBJ)
	$(ARCHIVE_CMD) $(ARCHIVE_OPTS) $(STAT_LIB) $(OBJ)

%.o: %.c
	@$(CC) -fPIC -o $@ -c $< $(CFLAGS)
	@echo "  CC  $@"

clean:
	rm -f $(OBJ) $(BINARY) $(LIBNAME) $(SONAME)
