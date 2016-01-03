/*
 * anybus-tools is an application communicating with an HMS anybus module
 * through the driver anybus_interface.
 *
 * Copyright (C) 2011   Armadeus Systems
 *                      Kevin JOLY <joly.kevin25@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/in.h>

#include "edit_input.h"
#include "map_output.h"
#include "sel_iface.h"
#include "win.h"
#include "mailbox.h"
#include "read_reg.h"
#include "about_help.h"

void exit_curses()
{
    erase();
    refresh();
    curs_set(1);
    endwin();
    echo();
}

int init_file_ioctl(char *anybus_interface)
{
    char file_dev[256];

    snprintf(file_dev, sizeof(file_dev), "/dev/%s_device", anybus_interface);

    return open(file_dev, O_RDONLY);
}

int init_sock(char *anybus_interface)
{
    struct ifreq ifr;
    struct sockaddr_ll addr;
    int sk = 0;

    addr.sll_family = AF_PACKET;
    addr.sll_protocol = IPPROTO_UDP;
    addr.sll_pkttype = PACKET_BROADCAST;

    strncpy(ifr.ifr_name, anybus_interface, strlen(anybus_interface));

    if ((sk = socket(PF_PACKET , SOCK_RAW, AF_PACKET)) < 0 ) {
        exit_curses();
        perror("socket");
        return sk;
    }

    ioctl(sk, SIOCGIFINDEX, &ifr);
    addr.sll_ifindex = ifr.ifr_ifindex;

    if(bind(sk, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        exit_curses();
        perror("bind");
        return EXIT_FAILURE;
    }

return sk;
}

int print_menu(char *anybus_interface)
{
    char *choices[] = { "Edit input area", "Map output area", "Mailbox message", "Read register", "Change interface","About...", "Help" };
    int startx, starty, width, height;
    int ch = 0, selection = 0;
    WINDOW *win_menu;

    height = 12;
    width = 40;
    starty = 3;
    startx = (COLS - width) / 2;

    win_menu = create_new_win(height, width, starty, startx, "Anybus-tools");

    mvwprintw(win_menu, height - 2, width - 11, "'q' : quit");
    mvwprintw(win_menu, height - 2, 2, "interface: %s", anybus_interface);
    refresh_menu(win_menu, selection, width, choices, 7);
    wrefresh(win_menu);

    while((ch != 'q') && (ch != 'Q') && (ch != 10))
    {
        ch = wgetch(win_menu);
        switch(ch)
        {
            case 65:
                if (selection > 0)
                    selection--;
                break;
            case 66:
                if (selection < 6)
                    selection++;
                break;
        }

        refresh_menu(win_menu, selection, width, choices, 7);
        wrefresh(win_menu);
    }

    if ((ch == 'q') || (ch == 'Q'))
        selection = -1;
    endwin();

    return selection;
}

void signal_handler(int sig)
{
    switch(sig) {
        case SIGTERM:
            exit_curses();
            exit(0);
            break;
        default:
            break;
    }
}

int main()
{
    int ret = 0;
    char anybus_interface[IFACE_NAME_LEN];
    char *anybus_interface_ptr;
    int sock, fd_device;

    signal(SIGTERM, signal_handler);
    initscr();
    raw();
    noecho();
    curs_set(0);

    if ((anybus_interface_ptr = sel_iface()) == NULL) {
        exit_curses();
        return EXIT_FAILURE;
    }

    strncpy(anybus_interface, anybus_interface_ptr, IFACE_NAME_LEN);

    free(anybus_interface_ptr);

    if ((sock = init_sock(anybus_interface)) < 0)
        return EXIT_FAILURE;

    if((fd_device = init_file_ioctl(anybus_interface)) < 0) {
        close(sock);
        exit_curses();
        printf("Unable to find the device file of the anybus module\n");
        return fd_device;
    }

    while (ret >= 0) {

        erase();
        refresh();

        ret = print_menu(anybus_interface);

        erase();
        refresh();

        switch(ret) {
            case 0:
                if (edit_input(sock, fd_device) < 0) {
                    exit_curses();
                    perror("edit_input");
                    close(fd_device);
                    close(sock);
                    return EXIT_FAILURE;
                }
            break;
            case 1:
                if (map_output(sock, fd_device) < 0) {
                    exit_curses();
                    perror("map_output");
                    close(fd_device);
                    close(sock);
                    return EXIT_FAILURE;
                }
            break;
            case 2:
                if (mailbox(fd_device) < 0) {
                    exit_curses();
                    perror("mailbox");
                    close(fd_device);
                    close(sock);
                    return EXIT_FAILURE;
                }
            break;
            case 3:
                if (read_register(fd_device) < 0) {
                    exit_curses();
                    perror("read_register");
                    close(fd_device);
                    close(sock);
                    return EXIT_FAILURE;
                }
            break;
            case 4:
                if ((anybus_interface_ptr = sel_iface()) != NULL) {
                    strcpy(anybus_interface, anybus_interface_ptr);
                    free(anybus_interface_ptr);
                }
            break;
            case 5:
                about();
            break;
            case 6:
                help();
            break;
        }
    }

    close(fd_device);
    close(sock);
    exit_curses();
    return EXIT_SUCCESS;
}
