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

#define CATCH_CONFIG_MAIN

#include <cstdlib>
#include <exception>
#include <random>

#include <catch2/catch.hpp>

#include <clients/Client.hpp>

using Catch::Matchers::VectorContains;

int random_int_in_range(int a, int b) {
    static std::random_device rd; // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());       // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(a, b);

    return dis(gen);
}

std::string get_access_token() {
    char *env = std::getenv("FEEDLY_ACCESS_TOKEN");
    if (env == nullptr) {
        throw std::runtime_error("missing access token");
    }

    return env;
}

std::string get_user_id() {
    char *env = std::getenv("FEEDLY_USER_ID");
    if (env == nullptr) {
        throw std::runtime_error("missing user id");
    }

    return env;
}

feedly::DeveloperTokenCredentials get_token() {
    return {.developer_token = get_access_token(), .user_id = get_user_id()};
};

SCENARIO("Subscribe to a feed") {
    GIVEN("An authenticated client") {
        feedly::Client client{get_token()};

        WHEN("it makes a subscription request") {
            std::string url = "https://www.eff.org/rss/updates.xml";
            auto feed = client.subscribe({.website = url});

            THEN("it returns a feed") {
                REQUIRE_FALSE(feed.id.empty());
                REQUIRE(feed.website == url);
            }
        }
    }
}

SCENARIO("List subscriptions") {
    GIVEN("An authenticated client") {
        feedly::Client client{get_token()};

        WHEN("it subscribes to a feed") {
            std::string url = "https://www.eff.org/rss/updates.xml";
            auto feed = client.subscribe({.website = url});

            AND_WHEN("it lists all feeds") {
                auto feeds = client.subscriptions();

                THEN("it returns the feed it subscribed to") { REQUIRE_THAT(feeds, VectorContains(feed)); }
            }
        }
    }
}

SCENARIO("Subscribe to a feed and set custom title") {
    GIVEN("An authenticated client") {
        feedly::Client client{get_token()};

        WHEN("it makes a subscription request with a custom title") {
            std::string url = "https://www.eff.org/rss/updates.xml";
            auto feed = client.subscribe({.title = "My Custom Title", .website = url});

            THEN("it returns a feed") {
                REQUIRE_FALSE(feed.id.empty());
                REQUIRE(feed.website == url);
                REQUIRE(feed.title == "My Custom Title");
            }
        }
    }
}

SCENARIO("Subscribe to a feed with a category") {
    GIVEN("An authenticated client") {
        feedly::Client client{get_token()};

        WHEN("it creates a category") {
            auto ctg = client.create_category({.label = "test" + std::to_string(random_int_in_range(0, 999999999))});

            AND_WHEN("it subscribes to a feed given the category") {
                std::string url = "https://www.eff.org/rss/updates.xml";
                auto feed = client.subscribe({.website = url}, {{.id = ctg.id}});

                AND_WHEN("it lists the feeds by that category") {
                    auto feeds = client.subscriptions(ctg.id);

                    THEN("it returns a list containing the subscribed feed") {
                        REQUIRE_THAT(feeds, VectorContains(feed));
                    }
                }
            }
        }
    }
}

SCENARIO("Get Categories") {
    GIVEN("An authenticated client") {
        feedly::Client client{get_token()};

        WHEN("it creates a category") {
            auto ctg = client.create_category({.label = "test" + std::to_string(random_int_in_range(0, 999999999))});

            AND_WHEN("it gets all categories") {
                auto ctgs = client.categories();

                THEN("it returns a list of categories containing the created category") {
                    REQUIRE(!ctgs.empty());

                    REQUIRE_THAT(ctgs, VectorContains(ctg));
                }
            }
        }
    }
}

SCENARIO("Fetch entries from stream") {
    GIVEN("An authenticated client") {
        feedly::Client client{get_token()};

        WHEN("it subscribes to a feed") {
            std::string url = "https://www.eff.org/rss/updates.xml";
            auto feed = client.subscribe({.website = url});

            AND_WHEN("it fetches a stream for the feed") {
                auto entries = client.stream(feed.id);

                THEN("the entries list should be non empty") { REQUIRE(!entries.empty()); }
            }
        }
    }
}
