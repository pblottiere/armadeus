/*
 * Some basics menu operations with ncurses...
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
#include "win.h"

WINDOW *create_new_win(int height, int width, int starty, int startx, const char *title)
{
    WINDOW *win;
    int i;

    win = newwin(height, width, starty, startx);
    box(win, 0, 0);

    wattron(win, A_STANDOUT);

    for (i = 1 ; i < width - 1; i++)
        mvwprintw(win, 0, i, " ");

    mvwprintw(win, 0, (width - strlen(title)) / 2, title);

    wattroff(win, A_STANDOUT);

    wrefresh(win);

    return win;
}

void refresh_menu(WINDOW *win, int selection, int width, char **choices, int n_choices)
{
    int i;

    for (i = 0; i < n_choices ; i++) {
        if ( i == selection)
            wattron(win, A_BOLD);
           mvwprintw(win, 2 + i, ((width - strlen(choices[i])) / 2) - 2, " ");
        mvwprintw(win, 2 + i, (width - strlen(choices[i])) / 2, choices[i]);
        if ( i == selection)
            wattroff(win, A_BOLD);
    }

    mvwprintw(win, 2 + selection, (width - strlen(choices[selection])) / 2 - 2, "*");
    wmove(win, 2 + selection, ((width - strlen(choices[selection])) / 2) - 2);
}

void refresh_form_mail_to_send(WINDOW *win, int selection, int width, struct mailbox_msg mail, char focused)
{
    int i;
    char *choices[] = { "Message ID", "Message information", "Command Number", "Data size","Extended Word 1", "Extended Word 2",
        "Extended Word 3", "Extended Word 4", "Extended Word 5", "Extended Word 6", "Extended Word 7", "Extended Word 8", "Data"};

    for (i = 0; i < 14 ; i++) {

        if ( i < 13)
            mvwprintw(win, 2 + i, 2, choices[i]);

        if ((i == selection) && (focused == 1))
            wattron(win, A_STANDOUT);

        switch(i) {
            case 0:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.id);
            break;
            case 1:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.msg_information);
            break;
            case 2:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.command_number);
            break;
            case 3:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.data_size);
            break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.extended_word[i - 4]);
            break;
            case 12:
                mvwprintw(win, 2 + i, width - 9, "< EDIT >");
            break;
            case 13:
                 mvwprintw(win, 3 + i, (width - strlen("< SEND >")) / 2, "< SEND >");
            break;
        }

        if ((i == selection) && (focused == 1))
            wattroff(win, A_STANDOUT);
    }

}

void refresh_form_received_mail(WINDOW *win, int selection, int width, struct mailbox_msg mail, char focused)
{
    int i;
    char *choices[] = { "Message ID", "Messages informations", "Command Number", "Data size","Extended Word 1", "Extended Word 2",
        "Extended Word 3", "Extended Word 4", "Extended Word 5", "Extended Word 6", "Extended Word 7", "Extended Word 8", "Data"};

    for (i = 0; i < 13 ; i++) {

        mvwprintw(win, 2 + i, 2, choices[i]);

        if ((i == selection) && (focused == 1))
            wattron(win, A_STANDOUT);

        switch(i) {
            case 0:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.id);
            break;
            case 1:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.msg_information);
            break;
            case 2:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.command_number);
            break;
            case 3:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.data_size);
            break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                mvwprintw(win, 2 + i, width - 5, "%04X", mail.extended_word[i - 4]);
            break;
            case 12:
                mvwprintw(win, 2 + i, width - 9, "< VIEW >");
        }

        if ((i == selection) && (focused == 1))
            wattroff(win, A_STANDOUT);
    }

}
