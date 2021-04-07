/**
 * Copyright (C) 2COLOR_BLACK21 Jorge Martinez Hernandez <me@jmartinezhern.com>
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

#include <curses.h>
#include <string>

#include <cui/views/Application.hpp>

Application::Application(int exit_key) : m_exit_key(exit_key) {
    initscr();

    start_color();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_GREEN, COLOR_BLACK);
    init_pair(8, COLOR_YELLOW, COLOR_BLACK);

    assume_default_colors(-1, -1);

    // Have to do this first or the stdscr will be drawn after we have already drawn menus,
    // causing getch to refresh stdscr and clear everything.
    refresh();
}

Application::~Application() { endwin(); }

void Application::add_component(Component *cmpt) { m_components.push_back(cmpt); }

void Application::run() {
    int key;

    m_cmpt_iter = m_components.begin();

    m_cmpt_in_focus = *m_cmpt_iter;

    while ((key = getch()) != m_exit_key) {
        switch (key) {
        case KEY_LEFT:
            m_cmpt_in_focus = prev_cmpnt();
            break;
        case KEY_RIGHT:
            m_cmpt_in_focus = next_cmpnt();
            break;
        default:
            m_cmpt_in_focus->on_key(key);
        }
    }
}

Component *Application::prev_cmpnt() {
    m_cmpt_iter--;

    if (m_cmpt_iter == m_components.begin() - 1) {
        m_cmpt_iter = m_components.end() - 1;
    }

    return *m_cmpt_iter;
}

Component *Application::next_cmpnt() {
    m_cmpt_iter++;

    if (m_cmpt_iter == m_components.end()) {
        m_cmpt_iter = m_components.begin();
    }

    return *m_cmpt_iter;
}
