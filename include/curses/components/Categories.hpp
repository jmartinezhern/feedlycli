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

#include <vector>

#include <menu.h>

#undef OK

#include <clients/Client.hpp>

#ifndef FEEDLYCLI_CURSES_COMPONENTS_CATEGORIES_HPP
#define FEEDLYCLI_CURSES_COMPONENTS_CATEGORIES_HPP

namespace curses {

class Categories {
  public:
    Categories() = default;

    virtual ~Categories();

    void render();

  private:
    feedly::Categories m_categories;
    WINDOW *m_menu_win = nullptr;
    MENU *m_menu = nullptr;
    std::vector<ITEM*> m_menu_items;
};

} // namespace curses

#endif // FEEDLYCLI_CURSES_COMPONENTS_CATEGORIES_HPP
