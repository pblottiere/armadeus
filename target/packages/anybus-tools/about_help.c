/*
 * Display help and about windows in anybus-tools application.
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
#include "about_help.h"

void about()
{
    int height = 13;
    int width = 45;
    int starty = 2;
    int startx = (COLS -width) / 2;
    int i;
    WINDOW* win;

    win = newwin(height, width, starty, startx);
    box(win, 0, 0);

    wattron(win, A_STANDOUT);
    for (i = 1 ; i < width - 1; i++)
        mvwprintw(win, 0, i, " ");

    mvwprintw(win, 0, (width - strlen("About...")) / 2, "About...");
    wattroff(win, A_STANDOUT);

    mvwprintw(win, 2, 2, "Anybus tools is a set of tools used");
    mvwprintw(win, 3, 2, "to communicate with the HMS Anybus module.");
    mvwprintw(win, 5, 2, "(C) Copyright 2011 - Armadeus Systems");
    mvwprintw(win, 6, 2, "<support@armadeus.com>");
    mvwprintw(win, 7, 2, "Author: Kevin JOLY joly.kevin25@gmail.com");
    mvwprintw(win, 9, 2, "HMS website : http://www.hms.se/");
    wattroff(win, A_STANDOUT);

    wattron(win, A_STANDOUT);
    mvwprintw(win, height - 2, (width - strlen("< Ok >")) / 2, "< Ok >");
    wattroff(win, A_STANDOUT);

    wmove(win, height - 2, (width - strlen("< Ok >")) / 2);
    wrefresh(win);
    getch();
    endwin();
}

void help()
{
    int height = 24;
    int width = 78;
    int starty = 0;
    int startx = (COLS -width) / 2;
    int i;
    WINDOW* win;

    win = newwin(height, width, starty, startx);
    box(win, 0, 0);

    wattron(win, A_STANDOUT);
    for (i = 1 ; i < width - 1; i++)
        mvwprintw(win, 0, i, " ");

    mvwprintw(win, 0, (width - strlen("Help")) / 2, "Help");
    wattroff(win, A_STANDOUT);

    wattron(win, A_BOLD);
    mvwprintw(win, 1, 2, "Edit input area");
    wattroff(win, A_BOLD);
    mvwprintw(win, 2, 2, "Send datas to the fieldbus through the HMS Anybus module.");
    mvwprintw(win, 3, 2, "You can move the cursor in the Anybus's  internal memory using arrow key.");
    mvwprintw(win, 4, 2, "Use < Enter > to edit the selected field.");

    wattron(win, A_BOLD);
    mvwprintw(win, 5, 2, "Map output area");
    wattroff(win, A_BOLD);
    mvwprintw(win, 6, 2, "View datas on the fieldbus through the HMS Anybus module.");
    mvwprintw(win, 7, 2, "You can move the cursor in the Anybus's  internal memory using arrow key.");

    wattron(win, A_BOLD);
    mvwprintw(win, 8, 2, "Mailbox message");
    wattroff(win, A_BOLD);
    mvwprintw(win, 9, 2, "Send and receive Anybus mailbox messages.");
    mvwprintw(win, 10, 2, "You can move the cursor on the differents fields using arrow key.");
    mvwprintw(win, 11, 2, "Use < Enter > to edit the selected field. You can edit the mailbox datas");
    mvwprintw(win, 12, 2, "as the Edit input area menu. Then, move to < Send >  to send the mailbox.");
    mvwprintw(win, 13, 2, "When a response occurs, the right window is automatically refreshed.");

    wattron(win, A_BOLD);
    mvwprintw(win, 14, 2, "Read register");
    wattroff(win, A_BOLD);
    mvwprintw(win, 15, 2, "Read an anybus register. Hit < Enter > to edit the address.");

    wattron(win, A_BOLD);
    mvwprintw(win, 16, 2, "change interface");
    wattroff(win, A_BOLD);
    mvwprintw(win, 17, 2, "Change the Anybus interface corresponding to the module.");

    wattron(win, A_BOLD);
    mvwprintw(win, 18, 2, "About...");
    wattroff(win, A_BOLD);
    mvwprintw(win, 19, 2, "Display the \"About\" message.");

    wattron(win, A_BOLD);
    mvwprintw(win, 20, 2, "Help");
    wattroff(win, A_BOLD);
    mvwprintw(win, 21, 2, "Display this help.");

    wattron(win, A_STANDOUT);
    mvwprintw(win, height - 2, (width - strlen("< Ok >")) / 2, "< Ok >");
    wattroff(win, A_STANDOUT);

    wmove(win, height - 2, (width - strlen("< Ok >")) / 2);
    wrefresh(win);
    getch();
    endwin();
}
