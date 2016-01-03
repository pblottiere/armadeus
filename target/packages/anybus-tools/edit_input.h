#ifndef EDIT_INPUT_HEADER
#define EDIT_INPUT_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <net/anybus_interface.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

int edit_input(int sock, int fd_device);

#endif
