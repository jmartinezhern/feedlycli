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

#include <cui/views/PostsView.hpp>
#include <cui/views/utils.hpp>

PostsView::PostsView(feedly::Client client) : m_client(std::move(client)) {
    m_nc_menu = new_menu(nullptr);

    int height, width;

    getmaxyx(stdscr, height, width);

    int sub_width = (width * 2) / 3;

    m_nc_window = newwin(height - 2, sub_width, 0, width - sub_width);
    keypad(m_nc_window, true);

    getmaxyx(m_nc_window, height, width);

    set_menu_win(m_nc_menu, m_nc_window);
    set_menu_sub(m_nc_menu, derwin(m_nc_window, height - 4, width - 2, 3, 1));
    set_menu_format(m_nc_menu, height - 4, 0);

    set_menu_mark(m_nc_menu, "*");

    win_show(m_nc_window, "Posts", 1);

    menu_opts_off(m_nc_menu, O_SHOWDESC);

    post_menu(m_nc_menu);

    wrefresh(m_nc_window);
}

void PostsView::update(const std::string &ctgID) {
    m_posts = m_client.stream(ctgID);

    unpost_menu(m_nc_menu);

    set_menu_items(m_nc_menu, nullptr);

    for (auto const &item : m_nc_items) {
        free_item(item);
    }

    m_nc_items = std::vector<ITEM *>();

    for (const auto &post : m_posts) {
        m_nc_items.push_back(new_item(post.title.c_str(), post.id.data()));
    }

    m_nc_items.push_back(nullptr);

    set_menu_items(m_nc_menu, m_nc_items.data());

    post_menu(m_nc_menu);

    wrefresh(m_nc_window);
}

void PostsView::on_key(int key) {
    switch (key) {
    case KEY_DOWN:
        menu_driver(m_nc_menu, REQ_DOWN_ITEM);
        break;
    case KEY_UP:
        menu_driver(m_nc_menu, REQ_UP_ITEM);
        break;
    default:
        break;
    }

    wrefresh(m_nc_window);
}
