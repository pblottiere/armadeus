# Default values for local compiling:
ARMADEUS_BASE_DIR = ../../../
-include $(ARMADEUS_BASE_DIR)/Makefile.in
CFLAGS = -Wall -g -O0
LDFLAGS=
CC = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
CXX = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)g++
INSTALL_DIR = $(ARMADEUS_STAGING_DIR)

C_BINARY = test_cdevices
C_LIBNAME = libas_devices.so
C_LIB = c/$(C_LIBNAME)
C_INCLUDE = c

P_WRAPPER = .python_wrappers

CPP_BINARY = test_cppdevices
CPP_LIBNAME = libas_devices_cpp.so
CPP_LIB = cpp/$(CPP_LIBNAME)
CPP_INCLUDE = cpp


.PHONY: all install install-exe clean

all: $(C_BINARY) $(P_WRAPPER) $(CPP_BINARY)

c_target: $(C_BINARY)

cpp_target: $(CPP_BINARY)

python_target: $(P_WRAPPER)

$(C_BINARY): main.c test_c.h $(C_LIB)
	$(CC) $(CFLAGS) main.c $(C_LIB) -o $(C_BINARY) -I$(C_INCLUDE) -lm

$(CPP_BINARY): main.c test_cpp.h $(CPP_LIB) $(C_LIB)
	$(CXX) $(CFLAGS) main.c $(C_LIB) $(CPP_LIB) -I$(C_INCLUDE) -I$(CPP_INCLUDE) -o $(CPP_BINARY)
#-D__cplusplus is automatically defined by compiler

$(CPP_LIB): $(C_LIB)
	+make -C cpp/

$(C_LIB):
	+make -C c/

$(P_WRAPPER): $(C_LIB)
	+make -C python/ STAGING_DIR=$(ARMADEUS_STAGING_DIR)
	touch $(P_WRAPPER)

install:
	install -D $(C_LIB) $(INSTALL_DIR)/usr/lib/$(C_LIBNAME)
	mkdir -p $(INSTALL_DIR)/usr/include/as_devices/
	cp $(C_INCLUDE)/*.h $(INSTALL_DIR)/usr/include/as_devices/

install-exe:
	install -D $(C_LIB) $(INSTALL_DIR)/usr/lib/$(C_LIBNAME)

clean:
	make -C c/ clean
	make -C cpp/ clean
	rm -f $(P_WRAPPER)
	make -C python/ clean
	rm -f $(C_BINARY) $(CPP_BINARY)
