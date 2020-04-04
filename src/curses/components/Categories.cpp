/**
 * feedlycli
 * Copyright (C) 2020 Jorge Martinez Hernandez <me@jmartinezhern.com>
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

#include <curses/components/Categories.hpp>

namespace curses {
Categories::~Categories() {
    if (!m_menu_items.empty()) {
        for (auto item : m_menu_items) {
            free_item(item);
        }
    }

    if (m_menu) {
        unpost_menu(m_menu);
        free_menu(m_menu);
    }
}

void Categories::render() {
    m_categories = {{.id = "1", .label = "All"}, {.id = "2", .label = "News"}};

    std::transform(m_categories.begin(), m_categories.end(), std::back_inserter(m_menu_items),
                   [](const feedly::Category &ctg) { return new_item(ctg.label.c_str(), ctg.id.c_str()); });

    ITEM *items[3];
    std::copy(m_menu_items.begin(), m_menu_items.end(), items);

    items[2] = nullptr;

    m_menu = new_menu(items);
    m_menu_win = newwin(LINES - 2, 40, 0, 0);
    keypad(m_menu_win, true);

    set_menu_win(m_menu, m_menu_win);
    set_menu_sub(m_menu, derwin(m_menu_win, 0, 38, 3, 1));

    set_menu_mark(m_menu, "  ");

    menu_opts_off(m_menu, O_SHOWDESC);
    menu_opts_on(m_menu, O_NONCYCLIC);

    post_menu(m_menu);

    wrefresh(m_menu_win);
}

} // namespace curses

int main() {
    initscr();

    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int ch = 0;

    refresh();

    curses::Categories categories;

    categories.render();

    while ((ch = getch()) != KEY_F(1)) {
    }

    endwin();
}
