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

#include <map>

#include <curses.h>
#include <menu.h>

#include <cui/views/CategoriesView.hpp>
#include <cui/views/PostsView.hpp>
#include <cui/views/utils.hpp>

CategoriesView::CategoriesView(feedly::Client client, PostsView *postsView)
    : m_client(std::move(client)), m_posts_view(postsView) {
    m_nc_menu = new_menu(nullptr);

    int height, width;

    getmaxyx(stdscr, height, width);

    m_nc_window = newwin(height - 2, width / 3, 0, 0);
    keypad(m_nc_window, true);

    getmaxyx(m_nc_window, height, width);

    set_menu_win(m_nc_menu, m_nc_window);
    set_menu_sub(m_nc_menu, derwin(m_nc_window, height - 4, width - 2, 3, 1));
    set_menu_format(m_nc_menu, height - 4, 0);

    set_menu_mark(m_nc_menu, "*");

    win_show(m_nc_window, "Categories", 2);

    menu_opts_off(m_nc_menu, O_SHOWDESC);

    post_menu(m_nc_menu);

    wrefresh(m_nc_window);
}

void CategoriesView::update() {
    m_categories = m_client.categories();

    unpost_menu(m_nc_menu);

    set_menu_items(m_nc_menu, nullptr);

    for (auto const &item : m_nc_items) {
        free_item(item);
    }

    m_nc_items = std::vector<ITEM *>();

    for (const auto &ctg : m_categories) {
        m_nc_items.push_back(new_item(ctg.label.c_str(), ctg.id.data()));
    }

    m_nc_items.push_back(nullptr);

    set_menu_items(m_nc_menu, m_nc_items.data());

    post_menu(m_nc_menu);

    wrefresh(m_nc_window);
}

int CategoriesView::current_item_idx() { return item_index(::current_item(m_nc_menu)); }

void CategoriesView::on_key(int key) {
    switch (key) {
    case KEY_DOWN:
        menu_driver(m_nc_menu, REQ_DOWN_ITEM);
        break;
    case KEY_UP:
        menu_driver(m_nc_menu, REQ_UP_ITEM);
        break;
    case 10:
        m_posts_view->update(m_categories[current_item_idx()].id);
        break;
    default:
        break;
    }

    wrefresh(m_nc_window);
}
