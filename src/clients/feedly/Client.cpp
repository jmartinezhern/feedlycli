/**
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

#include <cpr/util.h>
#include <nlohmann/json.hpp>

#include <clients/Client.hpp>

using json = nlohmann::json;

namespace feedly {
Client::Client(DeveloperTokenCredentials credentials)
    : m_creds{std::move(credentials)}, m_auth_header{cpr::Header{
                                           {"Authorization", "OAuth " + std::move(m_creds.developer_token)}}} {}

Feed Client::subscribe(const Feed &feed, const Categories &ctgs) const {
    json j;
    j["id"] = "feed/" + feed.website;
    if (feed.title) {
        j["title"] = *feed.title;
    }

    if (not ctgs.empty()) {
        for (const auto &ctg : ctgs) {
            j["categories"].push_back({{"label", ctg.label}, {"id", ctg.id}});
        }
    }

    auto header = m_auth_header;
    header["Content-Type"] = "application/json";

    auto r = cpr::Post(cpr::Url{std::string(s_url) + "/subscriptions"}, header, cpr::Body{j.dump()});

    if (r.status_code not_eq 200) {
        std::string error = "Could not add subscription: " + std::to_string(r.status_code);
        throw std::runtime_error(error.c_str());
    }

    auto resp = json::parse(r.text);

    return {resp[0]["id"], resp[0]["title"], resp[0]["website"], resp[0]["visualUrl"], resp[0]["updated"]};
}

Category Client::create_category(const Category &ctg) const {
    json j;
    j["label"] = ctg.label;

    auto header = m_auth_header;
    header["Content-Type"] = "application/json";

    auto r = cpr::Post(cpr::Url{std::string(s_url) + "/collections"}, header, cpr::Body{j.dump()});

    if (r.status_code not_eq 200) {
        std::string error = "Could not create category: " + std::to_string(r.status_code);
        throw std::runtime_error(error.c_str());
    }

    auto resp = json::parse(r.text);

    return {resp[0]["id"], resp[0]["label"]};
}

Categories Client::categories() const {
    auto r = cpr::Get(cpr::Url{std::string(Client::s_url) + "/categories"}, m_auth_header);

    if (r.status_code not_eq 200) {
        std::string error = "Could not get categories: " + std::to_string(r.status_code);
        throw std::runtime_error(error.c_str());
    }

    auto jsonResp = json::parse(r.text);

    Categories categories;
    for (auto &ctg : jsonResp) {
        categories.push_back({ctg["id"], ctg["label"]});
    }

    return categories;
}

Feeds Client::subscriptions() const {
    auto r = cpr::Get(cpr::Url{std::string(Client::s_url) + "/subscriptions"}, m_auth_header);

    if (r.status_code not_eq 200) {
        std::string error = "Could not get feeds: " + std::to_string(r.status_code);
        throw std::runtime_error(error.c_str());
    }

    auto resp = json::parse(r.text);

    Feeds feeds;
    for (auto &feed : resp) {
        feeds.push_back({feed["id"], feed["title"], feed["website"], feed["visualUrl"], feed["updated"]});
    }

    return feeds;
}

Feeds Client::subscriptions(const std::string &category_id) const {
    auto r = cpr::Get(cpr::Url{std::string(Client::s_url) + "/collections/" + cpr::util::urlEncode(category_id)},
                      m_auth_header);

    if (r.status_code not_eq 200) {
        std::string error = "Could not get subscriptions: " + std::to_string(r.status_code);
        throw std::runtime_error(error.c_str());
    }

    auto resp = json::parse(r.text);

    Feeds feeds;
    for (const auto &feed : resp[0]["feeds"]) {
        feeds.push_back({feed["id"], feed["title"], feed["website"], feed["visualUrl"], feed["updated"]});
    }

    return feeds;
}

} // namespace feedly
