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

#ifndef FEEDLYCLI_POSTSVIEW_HPP
#define FEEDLYCLI_POSTSVIEW_HPP

#include <curses.h>
#include <menu.h>

#undef OK

#include <clients/Client.hpp>
#include <cui/views/Component.hpp>

class PostsView : public Component {
  public:
    explicit PostsView(feedly::Client client);

    void update(const std::string &ctgID);

    void on_key(int key) override;

  private:
    feedly::Client m_client;

    feedly::Entries m_posts;

    std::vector<ITEM *> m_nc_items;

    WINDOW *m_nc_window = nullptr;
    MENU *m_nc_menu = nullptr;
};

#endif // FEEDLYCLI_POSTSVIEW_HPP
