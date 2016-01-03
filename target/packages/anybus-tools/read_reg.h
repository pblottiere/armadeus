#ifndef READ_REG_HEADER
#define READ_REG_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <net/anybus_interface.h>
#include <sys/ioctl.h>

int read_register(int fd_device);

#endif
