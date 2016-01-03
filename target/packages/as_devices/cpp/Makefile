# Default values for local compiling:
ARMADEUS_BASE_DIR = ../../../../
-include $(ARMADEUS_BASE_DIR)/Makefile.in
CXX = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)g++
CXXFLAGS =-Wall -g -O0

OBJS = as_i2c.o
OBJS += as_gpio.o
OBJS += as_dac.o
OBJS += as_adc.o
OBJS += as_pwm.o
OBJS += as_backlight.o
OBJS += as_93lcxx.o
OBJS += as_i2c_eeprom.o

OBJS += as_spi.o

LIBRARY = as_devices_cpp
LIB_VERS = 1
STAT_LIB = ${LIBRARY}.a
SO_NAME = lib${LIBRARY}.so
LIBNAME = ${SO_NAME}.${LIB_VERS}
ARCHIVE_CMD = ar
ARCHIVE_OPTS = -cvq

C_INCLUDE = ../c/
C_LIB = ../c/libas_devices.so.1

.PHONY: install install-exe clean

$(SO_NAME): ${OBJS}
	@echo "  LNK $@"
	@${CXX} -shared -Wl,-soname,${SO_NAME} -o ${SO_NAME} ${OBJS} ${C_LIB} -lc

${STAT_LIB}: ${OBJS} 
	${ARCHIVE_CMD} ${ARCHIVE_OPTS} ${STAT_LIB} ${OBJS}

%.o: %.cpp
	@echo "  CPP $@"
	@$(CXX) -fPIC -o $@ -c $< ${CXXFLAGS} -I${C_INCLUDE}
#-D__cplusplus is automatically defined by compiler

install: $(SO_NAME)
	mkdir -p $(INSTALL_DIR)/usr/lib
	install -D $(SO_NAME) $(INSTALL_DIR)/usr/lib
	cd $(INSTALL_DIR)/usr/lib/; ln -sf $(SO_NAME) $(LIBNAME)
	mkdir -p $(INSTALL_DIR)/usr/include/as_devices/cpp
	cp *.hpp $(INSTALL_DIR)/usr/include/as_devices/cpp

install-exe: $(SO_NAME)
	install -D $(SO_NAME) $(INSTALL_DIR)/usr/lib
	cd $(INSTALL_DIR)/usr/lib; ln -sf $(SO_NAME) $(LIBNAME)

clean:
	rm -f *.o *.so
	rm -f ${BINARY} ${STAT_LIB} ${LIBNAME}
