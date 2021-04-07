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

#ifndef FEEDLYCLI_CATEGORIESVIEW_HPP
#define FEEDLYCLI_CATEGORIESVIEW_HPP

#include <vector>

#include <menu.h>

#include <cui/views/Component.hpp>
#include <cui/views/PostsView.hpp>

#undef OK

#include <clients/Client.hpp>

class CategoriesView : public Component {
  public:
    CategoriesView(feedly::Client client, PostsView *postsView);

    void update();

    void on_key(int key) override;

    int current_item_idx();

  private:
    feedly::Client m_client;

    feedly::Categories m_categories;

    std::vector<ITEM *> m_nc_items;

    PostsView *m_posts_view = nullptr;
    WINDOW *m_nc_window = nullptr;
    MENU *m_nc_menu = nullptr;
};

#endif // FEEDLYCLI_CATEGORIESVIEW_HPP
