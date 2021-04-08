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

#include <optional>
#include <string>
#include <vector>

#include <cpr/cpr.h>

namespace feedly {

struct DeveloperTokenCredentials {
    std::string developer_token;
    std::string user_id;
};

struct Feed {
    std::string id;
    std::optional<std::string> title;
    std::string website;
    std::string visual_url;
    long updated = 0;
    long added = 0;

    bool operator==(const Feed &f) const { return id == f.id && website == f.website; }
};

using Feeds = std::vector<Feed>;

struct Category {
    std::string id;
    std::string label;

    bool operator==(const Category &b) const { return id == b.id && label == b.label; }
};

using Categories = std::vector<Category>;

struct Entry {
    std::string id;
    std::string title;
    std::string content;
    std::string origin_id;
    std::string origin_url;
    std::string origin_title;
};

using Entries = std::vector<Entry>;

enum class Rank { Newest, Oldest, Engagement };

struct Page {
    Rank rank;
    bool sort_by_oldest;
    bool unread_only;
    unsigned int count;
    unsigned int newer_than;
    std::string continuation_id;
};

class Client {
  public:
    static const Page default_page;

    explicit Client(DeveloperTokenCredentials credentials, std::string url = "https://cloud.feedly.com/v3");

    [[nodiscard]] Feed subscribe(const Feed &feed, const Categories &ctgs = {}) const;

    [[nodiscard]] Category create_category(const Category &ctg) const;

    [[nodiscard]] Categories categories() const;

    [[nodiscard]] Feeds subscriptions() const;

    [[nodiscard]] Feeds subscriptions(const std::string &category_id) const;

    [[nodiscard]] Entries stream(const std::string &stream_id, const Page &page = Client::default_page) const;

    [[nodiscard]] Entries all_stream(const Page &page = Client::default_page) const {
        return stream("user/" + m_creds.user_id + "/category/global.all", page);
    }

    [[nodiscard]] Entries uncategorized_stream(const Page &page = Client::default_page) const {
        return stream("user/" + m_creds.user_id + "/category/global.uncategorized", page);
    }

    [[nodiscard]] Entries saved_stream(const Page &page) const {
        return stream("user/" + m_creds.user_id + "/tag/global.saved", page);
    }

  private:
    [[nodiscard]] static cpr::Parameters page_parameters(const Page &page);

    std::string m_url;

    DeveloperTokenCredentials m_creds;

    cpr::Header m_auth_header;
};

} // namespace feedly

#endif // FEEDLYCLI_CLIENTS_FEEDLY_HPP
