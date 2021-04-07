/**
 * Copyright (C) 2021 Jorge Martinez Hernandez <me@jmartinezhern.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstring>
#include <memory>

#include <curses.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width, const char *str) {
    int x, y;

    getyx(win, y, x);

    if (startx != 0) {
        x = startx;
    }

    if (starty != 0) {
        y = starty;
    }

    if (width == 0) {
        width = 80;
    }

    x = (int)(startx + ((width - strlen(str)) / 2));

    mvwprintw(win, y, x, "%s", str);
}

void print_in_center(WINDOW *win, int starty, int startx, int height, int width, char *str, chtype color) {
    int length, x, y;

    getyx(win, y, x);

    if (width == 0) {
        width = 80;
    }

    length = strlen(str);

    x = startx + (int)((width - length) / 2);
    y = starty + (int)(height / 2);

    wattron(win, color);
    mvwprintw(win, y, x, "%s", str);
    wattroff(win, color);
}

void win_show(WINDOW *win, const char *label, int label_color = 2) {
    int startx, starty, height, width;

    getbegyx(win, starty, startx);
    getmaxyx(win, height, width);

    wattron(win, COLOR_PAIR(label_color));

    box(win, 0, 0);

    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, width - 2);
    mvwaddch(win, 2, width - 1, ACS_RTEE);

    print_in_middle(win, 1, 0, width, label);

    wattroff(win, COLOR_PAIR(label_color));
}
