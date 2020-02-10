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

#ifndef FEEDLYCLI_CLIENTS_FEEDLY_HPP
#define FEEDLYCLI_CLIENTS_FEEDLY_HPP

#include <string>
#include <vector>

#include <cpr/cpr.h>

namespace feedly {

struct Credentials {
    std::string user_id;
};

struct DeveloperTokenCredentials : Credentials {
    std::string developer_token;
    std::string user_id;
};

struct Category {
    std::string id = "";
    std::string label = "";

    bool operator==(const Category &b) const {
        return id == b.id && label == b.label;
    }
};

using Categories = std::vector<Category>;

class Client {
  public:
    explicit Client(DeveloperTokenCredentials credentials);

    Category create_category(const Category &ctg) const;

    [[nodiscard]] Categories categories() const;

  private:
    static constexpr const char *s_url = "https://cloud.feedly.com/v3";

    DeveloperTokenCredentials m_creds;

    cpr::Header m_auth_header;
};

} // namespace feedly

#endif // FEEDLYCLI_CLIENTS_FEEDLY_HPP
