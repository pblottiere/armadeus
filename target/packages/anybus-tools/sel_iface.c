/*
 * Select the anybus interface used for communicate with the HMS Anybus module
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
#include "sel_iface.h"

int is_anybus(char *iface_name)
{
    char buffer[IFACE_NAME_LEN];

    strncpy(buffer, iface_name, strlen(iface_name) - 1);

    if(strcmp(buffer, "anybus")) /* False */
        return 0;
    else
        return 1;
}

int print_menu_iface(char **ifaces_names,int n_ifaces)
{
    int startx, starty, width, height;
    int ch = 0, selection = 0;
    WINDOW *win_menu_iface;

    if (n_ifaces < 1) {
        mvprintw(2, 2, "/!\\ No anybus interface found.");
        refresh();
        return -1;
    }

    height = 12;
    width = 40;
    starty = 3;
    startx = (COLS - width) / 2;

    win_menu_iface = create_new_win(height, width, starty, startx, "Select anybus interface");

    mvwprintw(win_menu_iface, height - 2, 2, "'q' : quit");
    refresh_menu(win_menu_iface, selection, width, ifaces_names, n_ifaces);
    wrefresh(win_menu_iface);

    while((ch != 'q') && (ch != 'Q') && (ch != 10)) {
        ch = wgetch(win_menu_iface);
        switch(ch)
        {
            case 65:
                if (selection > 0)
                    selection--;
                break;
            case 66:
                if (selection < n_ifaces -1)
                    selection++;
                break;
        }

        refresh_menu(win_menu_iface, selection, width, ifaces_names, n_ifaces);
        wrefresh(win_menu_iface);
    }

    if ((ch == 'q') || (ch == 'Q'))
        selection = -1;
    endwin();

    return selection;
}

char* sel_iface()
{
    FILE *f_net_dev;
    char *ifaces_names[IFACE_NAME_LEN];
    char buffer[IFACE_NAME_LEN];
    int n_ifaces = 0;
    int iface_char = 0;
    char read_char = 0;
    int i, selection;

    f_net_dev = fopen("/proc/net/dev", "r");

    if (f_net_dev == NULL) {
        printf("Unable to open /proc/net/dev\n");
        return NULL;
    }

    while(!feof(f_net_dev)) {
        read_char = getc(f_net_dev);

        if (read_char == '\n') {
            iface_char = 0; /* Reset the interface name cursor */
        } else if ((iface_char < IFACE_NAME_LEN)) { /* Prevent from buffer overflow */
            if (read_char == ':') /* End of interface name */
                read_char = '\0';
            if (read_char != ' ') { /* Do not copy whitespace */
                buffer[iface_char] = read_char;
                iface_char++; /* Increment the interface name cursor */
            }
            if (read_char == '\0') {
                if (is_anybus(buffer)) {
                    ifaces_names[n_ifaces] = (char*)malloc(IFACE_NAME_LEN);
                    strcpy(ifaces_names[n_ifaces], buffer);
                    n_ifaces++; /* Increment the interface cursor */
                }
            }
        }
    }

    fclose(f_net_dev);

    selection = print_menu_iface(ifaces_names, n_ifaces);

    /* Free useless ifaces_names */
    for(i = 0; i < n_ifaces; i++) {
        if (i != selection)
            free(ifaces_names[i]);
    }

    if (selection < 0)
        return NULL;

    return ifaces_names[selection];
}
