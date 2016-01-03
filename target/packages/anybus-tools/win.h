#ifndef WIN_HEADER
#define WIN_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <net/anybus_interface.h>

WINDOW *create_new_win(int height, int width, int starty, int startx, const char *title);
void refresh_menu(WINDOW *win, int selection, int width, char **choices, int n_choices);
void refresh_form_mail_to_send(WINDOW *win, int selection, int width, struct mailbox_msg mail, char focused);
void refresh_form_received_mail(WINDOW *win, int selection, int width, struct mailbox_msg mail, char focused);

#endif
