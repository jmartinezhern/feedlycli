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

#include <clients/Client.hpp>
#include <cui/views/Application.hpp>
#include <cui/views/CategoriesView.hpp>

std::string get_env(const std::string &name) {
    char *env = std::getenv(name.c_str());
    if (env == nullptr) {
        throw std::runtime_error("missing " + name);
    }

    return env;
}

feedly::DeveloperTokenCredentials get_credentials() {
    return {.developer_token = get_env("FEEDLY_ACCESS_TOKEN"), .user_id = get_env("FEEDLY_USER_ID")};
};

int main() {
    feedly::Client client{get_credentials()};

    Application app{KEY_F(1)};

    PostsView postsView(client);
    CategoriesView ctgView(client, &postsView);

    app.add_component(&ctgView);
    app.add_component(&postsView);

    ctgView.update();

    app.run();

    return 0;
}