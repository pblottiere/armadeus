#ifndef MAP_OUTPUT_HEADER
#define MAP_OUTPUT_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <net/anybus_interface.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

int map_output(int sock, int fd_device);

#endif
