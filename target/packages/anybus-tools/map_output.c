/*
 * Watch HMS Anybus output data area memory
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
#include "map_output.h"

int map_output(int sock, int fd_device)
{
    int i, line_offset, selection_line = 0, selection_col = 0, ret;
    char ch = 0;
    struct anybus_frame frame;
    fd_set readfs_list;

    memset (frame.data, 0, sizeof(frame.data));

    if ((ret = ioctl(fd_device, ANYBUS_MAP_OUTPUT, frame.data)) < 0) {
        return ret;
    }

    while ((ch != 'q') && (ch != 'Q')) {

        FD_ZERO(&readfs_list);
        FD_SET(0, &readfs_list);
        FD_SET(sock, &readfs_list);

        line_offset = 0;

        for (i = 0 ; i < 0x100 ; i++) {

            if ((i % 16 == 0) && (i != 0))
                line_offset++;

            if ((selection_col == i % 16) && (selection_line == line_offset))
                attron(A_STANDOUT);

            mvprintw(line_offset, (i % 16) * 5, "%04X", frame.data[i]);
            if ((selection_col == i % 16) && (selection_line == line_offset))
                attroff(A_STANDOUT);
        }

        attron(A_STANDOUT);
        for (i = 0 ; i < 80 ; i++) {
            mvprintw(23, i, " ");
        }
        mvprintw(23, 1, "Anybus address: 0x%03X", (32 * selection_line + selection_col * 2));
        mvprintw(23, 30, "Fieldbus address: 0x%02X", (16 * selection_line + selection_col));
        mvprintw(23, 60, "q : quit this tool");
        attroff(A_STANDOUT);

        move(selection_line, selection_col*5);
        refresh();

        if((ret = select(sock + 1, &readfs_list, NULL, NULL, NULL)) < 0)
        {
            return ret;
        }

        if(FD_ISSET(sock, &readfs_list)) {
            if((ret = recv(sock, (void *)&frame, sizeof(frame), 0)) < 0)
                return ret;
        }

        if(FD_ISSET(0, &readfs_list)) {
            ch = getch();
            switch(ch) {
                case 65: /* Up */
                    if (selection_line > 0)
                        selection_line--;
                break;
                case 66: /* Down */
                    if (selection_line < 15)
                        selection_line++;
                break;
                case 68: /* Left */
                    if (selection_col > 0)
                        selection_col--;
                break;
                case 67: /* Right */
                    if (selection_col < 15)
                        selection_col++;
                break;
            }
        }

    }

    return 0;
}
