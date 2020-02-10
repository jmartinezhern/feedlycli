# Copyright (C) 2020 Jorge Martinez Hernandez <me@jmartinezhern.com>
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

find_package(nlohmann_json 3.2.0 REQUIRED)

include(FetchContent)

set(USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)
set(BUILD_CPR_TESTS OFF CACHE BOOL "")

FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/whoshuu/cpr.git
)

FetchContent_GetProperties(cpr)
if (NOT cpr_POPULATED)
    FetchContent_Populate(cpr)
    add_subdirectory(${cpr_SOURCE_DIR} ${cpr_BINARY_DIR})
endif ()


add_library(FeedlyClient STATIC Client.cpp)
target_include_directories(FeedlyClient PRIVATE ${PROJECT_SOURCE_DIR}/include ${CPR_INCLUDE_DIRS})
target_link_libraries(FeedlyClient PRIVATE ${CPR_LIBRARIES} nlohmann_json::nlohmann_json)
