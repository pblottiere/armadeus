
OBJ= $(strip $(BOARD_NAME))_ios.o

LIBRARY=straton_ios

LIB_VERS=1
STAT_LIB=$(LIBRARY).a
SONAME=lib$(LIBRARY).so
LIBNAME=$(SONAME).$(LIB_VERS)
INCLUDE=-I$(STAGING_DIR)/usr/include/as_devices/
ARCHIVE_CMD=ar
ARCHIVE_OPTS=-cvq

$(LIBNAME): $(OBJ)
	$(CC) -shared -Wl,-soname,$(SONAME) -o $(SONAME) $(OBJ) -las_devices
	ln -sf $(SONAME) $(LIBNAME)

$(STAT_LIB): $(OBJ)
	$(ARCHIVE_CMD) $(ARCHIVE_OPTS) $(STAT_LIB) $(OBJ)

%.o: %.c
	$(CC) -fPIC -o $@ -c $< $(CFLAGS) $(INCLUDE)

clean:
	rm -f $(OBJ) $(BINARY) $(LIBNAME) $(SONAME)

