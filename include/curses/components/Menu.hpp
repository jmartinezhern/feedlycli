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

#include <menu.h>

#include <vector>

#ifndef FEEDLYCLI_MENU_HPP
#define FEEDLYCLI_MENU_HPP

namespace curses {
class Menu {
  public:
    class Item {
      public:
        Item(const Item &item) = default;

        [[nodiscard]] ITEM *get_curses_item() const { return m_item; };

      private:
        ITEM *m_item;
    };

    Menu();

    void set_items(const std::vector<Menu::Item> &items);

  private:
    MENU *m_menu;
};
} // namespace curses

#endif // FEEDLYCLI_MENU_HPP
