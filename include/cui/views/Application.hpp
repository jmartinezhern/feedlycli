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

#ifndef FEEDLYCLI_APPLICATION_HPP
#define FEEDLYCLI_APPLICATION_HPP

#include <iterator>
#include <vector>

#include <cui/views/Component.hpp>

class Application {
  public:
    explicit Application(int exit_key);

    virtual ~Application();

    void run();

    void add_component(Component *cmpt);

    Component *prev_cmpnt();

    Component *next_cmpnt();

  private:
    int m_exit_key;

    std::vector<Component *> m_components;

    std::vector<Component *>::iterator m_cmpt_iter;

    Component *m_cmpt_in_focus = nullptr;
};

#endif // FEEDLYCLI_APPLICATION_HPP
