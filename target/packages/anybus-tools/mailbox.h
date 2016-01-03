#ifndef MAILBOX_HEADER
#define MAILBOX_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <net/anybus_interface.h>
#include <sys/ioctl.h>

#include "win.h"

int mailbox(int fd_device);
int edit_data_mailbox(unsigned short data[ANYBUS_DATA_LEN], int data_len);
int map_data_mailbox(unsigned short data[ANYBUS_DATA_LEN], int data_len);

#endif
