# Copyright (C) 2021 Jorge Martinez Hernandez <me@jmartinezhern.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(CURSES_NEED_NCURSES TRUE)

find_package(Curses REQUIRED)
find_library(MENU_LIBRARY menu)
find_library(PANEL_LIBRARY panel)

add_library(CUIViews STATIC Application.cpp CategoriesView.cpp PostsView.cpp utils.cpp)
target_include_directories(CUIViews PRIVATE ${PROJECT_SOURCE_DIR}/include ${CPR_INCLUDE_DIRS} ${CURSES_INCLUDE_DIRS})
target_link_libraries(CUIViews PRIVATE FeedlyClient ${MENU_LIBRARY} ${PANEL_LIBRARY} ${CURSES_LIBRARIES})
