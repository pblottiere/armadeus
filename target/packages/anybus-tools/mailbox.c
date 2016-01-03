/*
 * Send and receive mailbox message from the HMS Anybus module
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
#include "mailbox.h"

int mailbox(int fd_device)
{
    int startx, starty, width, height, ycurpos, xcurpos;
    WINDOW *win_send_mb, *win_recv_mb;
    char focus = 0, ch;
    int selection = 0, i, ret = 0;
    struct mailbox_msg mail_to_send, received_mail;
    unsigned short value;
    fd_set fdlist;

    memset(&mail_to_send, 0, sizeof(mail_to_send));
    memset(&received_mail, 0, sizeof(received_mail));

    height = 19;
    width = 38;
    starty = 1;
    startx = 1;
init_windows:
    win_send_mb = create_new_win(height, width, starty, startx, "Mail to send");
    win_recv_mb = create_new_win(height, width, starty, startx + 39, "Response mail");

    attron(A_STANDOUT);
    for (i = 0 ; i < 80 ; i++) {
        mvprintw(23, i, " ");
    }

    mvprintw(23, 2, "<- or -> : switch window");
    mvprintw(23, 60, "q : quit this tool");
    attroff(A_STANDOUT);

    refresh_form_mail_to_send(win_send_mb, selection, width, mail_to_send, focus ^ 1);
    wrefresh(win_send_mb);
    refresh_form_received_mail(win_recv_mb, selection, width, received_mail, focus);
    wrefresh(win_recv_mb);
    refresh();


    while((ch != 'q') && (ch != 'Q')) {

        FD_ZERO(&fdlist);
        FD_SET(fd_device, &fdlist);
        FD_SET(0, &fdlist);

        if (selection == 13)
            ycurpos = starty + selection + 3;
        else
            ycurpos = starty + selection + 2;

        if (focus == 0) {
            if (selection == 12)
                xcurpos = startx + width - 7;
            else if (selection == 13)
                xcurpos = startx + width - 21;
            else
                xcurpos = startx + width - 5;
        } else {
            if (selection == 12)
                xcurpos = startx + 39 + width - 7;
            else
                xcurpos = startx + 39 + width - 5;
        }

        move(ycurpos, xcurpos);
        refresh();

        if (select(fd_device + 1, &fdlist, NULL, NULL, NULL) < 0)
            mvprintw(0, 0, "Error");

        if (FD_ISSET(0, &fdlist)) {
            ch = getch();
            switch(ch) {
                case 65: /* Up */
                    if (selection > 0)
                        selection--;
                break;
                case 66: /* Down */
                    if (((selection < 13) && (focus == 0)) || (selection < 12))
                        selection++;
                break;
                case 68: /* Left */
                    focus = 0;
                    selection = 0;
                break;
                case 67: /* Right */
                    focus = 1;
                    selection = 0;
                break;
                case 10:
                    if (focus == 0) {
                        if (selection < 12) {
                            mvwprintw(win_send_mb, selection + 2, width - 5, "    ");
                            echo();
                            mvwscanw(win_send_mb, selection + 2, width - 5, "%4X", &value);
                            noecho();
                        }
                        switch (selection) {
                            case 0:
                                mail_to_send.id = value;
                            break;
                            case 1:
                                mail_to_send.msg_information = value;
                            break;
                            case 2:
                                mail_to_send.command_number = value;
                            break;
                            case 3:
                                if (value > ANYBUS_MAILBOX_DATA_LEN)
                                    value = ANYBUS_MAILBOX_DATA_LEN;
                                mail_to_send.data_size = value;
                            break;
                            case 4:
                            case 5:
                            case 6:
                            case 7:
                            case 8:
                            case 9:
                            case 10:
                            case 11:
                                mail_to_send.extended_word[selection - 4] = value;
                            break;
                            case 12:
                                endwin();
                                edit_data_mailbox(mail_to_send.data, mail_to_send.data_size / 2);
                                goto init_windows;
                            break;
                            case 13:
                                if ((ret = ioctl(fd_device, ANYBUS_MB_WRITE, &mail_to_send)) < 0)
                                    return ret;
                            break;
                        }
                    } else {
                        endwin();
                        if ((ch == 10) && (selection == 12))
                            map_data_mailbox(received_mail.data, received_mail.data_size / 2);
                        goto init_windows;
                    }
                break;
            }
        }
        if (FD_ISSET(fd_device, &fdlist)) {
            if((ret = ioctl(fd_device, ANYBUS_MB_READ, &received_mail)) < 0)
                return ret;
        }

        refresh_form_mail_to_send(win_send_mb, selection, width, mail_to_send, focus ^ 1);
        wrefresh(win_send_mb);
        refresh_form_received_mail(win_recv_mb, selection, width, received_mail, focus);
        wrefresh(win_recv_mb);
    }

    endwin();

    return 0;
}

int edit_data_mailbox(unsigned short data[ANYBUS_DATA_LEN], int data_len)
{
    int i, line_offset, selection_line = 0, selection_col = 0;
    char ch = 0;
    int value;

    erase();
    refresh();

    if (data_len == 0) {
        mvprintw(2, 2, "/!\\ No datas available in this mailbox message.");
        mvprintw(3, 2, "    Press any key to continue.");
        getch();
        erase();
        refresh();
        return -1;
    }

    while ((ch != 'q') && (ch != 'Q'))
    {

        line_offset = 0;

        for (i = 0 ; i < data_len ; i++) {

            if ((i % 16 == 0) && (i != 0))
                line_offset++;

            mvprintw(line_offset, (i % 16) * 5 + 4, " ");

            if ((selection_col == i % 16) && (selection_line == line_offset))
                attron(A_STANDOUT);

            mvprintw(line_offset, (i % 16) * 5, "%04X", data[i]);

            if ((selection_col == i % 16) && (selection_line == line_offset))
                attroff(A_STANDOUT);
        }

        attron(A_STANDOUT);
        for (i = 0 ; i < 80 ; i++) {
            mvprintw(23, i, " ");
        }

        mvprintw(23, 1, "Address: 0x%03X", (32 * selection_line + selection_col * 2));
        mvprintw(23, 60, "q : quit this tool");
        attroff(A_STANDOUT);

        refresh();

        move(selection_line, selection_col*5);

        ch = getch();
        switch(ch)
        {
            case 65: /* Up */
                if (selection_line > 0)
                    selection_line--;
            break;
            case 66: /* Down */
                if (selection_line < ((data_len - 1)/ 16)) {
                    selection_line++;
                    if ((selection_line == ((data_len - 1)/ 16)) && (selection_col > (data_len - 1) % 16))
                        selection_col = (data_len - 1) % 16;
                }
            break;
            case 68: /* Left */
                if (selection_col > 0)
                    selection_col--;
            break;
            case 67: /* Right */
                if (((selection_line != ((data_len - 1)/ 16)) && selection_col < 15) || (selection_col < ((data_len - 1) % 16)))
                    selection_col++;
            break;
            case 10: /* Validate */
                mvprintw(selection_line, selection_col * 5, "    ");
                echo();
                mvscanw(selection_line, selection_col * 5, "%4X", &value);
                noecho();
                data[16 * selection_line + selection_col] = value;
            break;
        }
    }

    erase();
    refresh();

    return 0;
}

int map_data_mailbox(unsigned short data[ANYBUS_DATA_LEN], int data_len)
{
    int i, line_offset, selection_line = 0, selection_col = 0;
    char ch = 0;

    erase();
    refresh();

    if (data_len == 0) {
        mvprintw(2, 2, "/!\\ No datas available in this mailbox message.");
        mvprintw(3, 2, "    Press any key to continue.");
        getch();
        erase();
        refresh();
        return -1;
    }

    while ((ch != 'q') && (ch != 'Q'))
    {

        line_offset = 0;

        for (i = 0 ; i < data_len ; i++) {

            if ((i % 16 == 0) && (i != 0))
                line_offset++;

            mvprintw(line_offset, (i % 16) * 5 + 4, " ");

            if ((selection_col == i % 16) && (selection_line == line_offset))
                attron(A_STANDOUT);

            mvprintw(line_offset, (i % 16) * 5, "%04X", data[i]);

            if ((selection_col == i % 16) && (selection_line == line_offset))
                attroff(A_STANDOUT);
        }

        attron(A_STANDOUT);
        for (i = 0 ; i < 80 ; i++) {
            mvprintw(23, i, " ");
        }

        mvprintw(23, 1, "Address: 0x%03X", (32 * selection_line + selection_col * 2));
        mvprintw(23, 60, "q : quit this tool");
        attroff(A_STANDOUT);

        refresh();

        move(selection_line, selection_col*5);

        ch = getch();
        switch(ch)
        {
            case 65: /* Up */
                if (selection_line > 0)
                    selection_line--;
            break;
            case 66: /* Down */
                if (selection_line < ((data_len - 1)/ 16)) {
                    selection_line++;
                    if ((selection_line == ((data_len - 1)/ 16)) && (selection_col > (data_len - 1) % 16))
                        selection_col = (data_len - 1) % 16;
                }
            break;
            case 68: /* Left */
                if (selection_col > 0)
                    selection_col--;
            break;
            case 67: /* Right */
                if (((selection_line != ((data_len - 1)/ 16)) && selection_col < 15) || (selection_col < ((data_len - 1) % 16)))
                    selection_col++;
            break;
        }
    }

    erase();
    refresh();

    return 0;
}
