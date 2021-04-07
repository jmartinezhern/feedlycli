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

enable_testing()

find_package(nlohmann_json 3.2.0 REQUIRED)

include(FetchContent)

FetchContent_Declare(
        catch
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.11.1
)

FetchContent_GetProperties(catch)
if (NOT catch_POPULATED)
    FetchContent_Populate(catch)
    add_subdirectory(${catch_SOURCE_DIR} ${catch_BINARY_DIR})
endif ()

include(CTest)
set(CMAKE_MODULE_PATH "${catch_SOURCE_DIR}/contrib" ${CMAKE_MODULE_PATH})
include(Catch)

macro(package_add_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2 FeedlyClient nlohmann_json::nlohmann_json)
    target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${CPR_INCLUDE_DIRS})
    catch_discover_tests(${TEST_NAME}
            WORKING_DIRECTORY ${PROJECT_DIR}
            )
    set_target_properties(${TEST_NAME} PROPERTIES FOLDER ${PROJECT_SOURCE_DIR}/tests/unit)
endmacro()

add_subdirectory(acceptance)
#add_subdirectory(functional)
add_subdirectory(unit)
