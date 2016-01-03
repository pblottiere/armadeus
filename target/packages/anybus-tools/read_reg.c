/*
 * Read a specific in the HMS Anybus module.
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
#include "read_reg.h"

int read_register(int fd_device)
{
    WINDOW *win;
    struct anybus_register anybus_reg = {.address = 0, .value = 0 };
    int i, ret = 0;
    char ch;
    int height = 8;
    int width = 22;
    int starty = 2;
    int startx = (COLS -width) / 2;
    unsigned short value;

    win = newwin(height, width, starty, startx);
    box(win, 0, 0);

    wattron(win, A_STANDOUT);
    for (i = 1 ; i < width - 1; i++)
        mvwprintw(win, 0, i, " ");

    mvwprintw(win, 0, (width - strlen("Read register")) / 2, "Read register");
    wattroff(win, A_STANDOUT);

    mvwprintw(win, 2, 2, "Address :");
    mvwprintw(win, 3, 2, "Value :");
    mvwprintw(win, height - 2, 1, "q : quit this tool");

    while ((ch != 'q') && (ch != 'Q')) {
        wattron(win, A_STANDOUT);
        mvwprintw(win, 2, width - 4, "%03X", anybus_reg.address);
        wattroff(win, A_STANDOUT);
        mvwprintw(win, 3, width - 3, "%02X", anybus_reg.value);

        wmove(win, 2, width-4);
        wrefresh(win);
        ch = getch();

        if (ch == 10) {
            mvwprintw(win, 2, width - 4, "   ");
            echo();
            mvwscanw(win, 2, width - 4, "%3X", &value);
            noecho();
            anybus_reg.address = value;
            if ((ret = ioctl(fd_device, ANYBUS_READ_REG, &anybus_reg)) < 0) {
                return ret;
            }
        }
    }

    endwin();
    return 0;
}
