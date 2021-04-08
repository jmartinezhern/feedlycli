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

#include <future>

#include <catch2/catch.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include <clients/Client.hpp>

using Catch::Matchers::Equals;
using nlohmann::json;

feedly::DeveloperTokenCredentials mock_creds{
    .developer_token = "developer_token",
    .user_id = "user-id",
};

std::string mock_server_host{"http://localhost:1234"};

SCENARIO("Subscribe to a feed") {
    GIVEN("A mock HTTP server") {
        httplib::Server svr;

        json data = {{
            {"categories",
             {{{"id", "user/c805fcbf-3acf-4302-a97e-d82f9d7c897f/category/design"}, {"label", "design"}},
              {{"id", "user/c805fcbf-3acf-4302-a97e-d82f9d7c897f/category/global.must"}, {"label", "must reads"}}}},
            {"id", "feed/http://feeds.feedburner.com/design-milk"},
            {"title", "Design Milk"},
            {"updated", 1367539068016},
            {"website", "http://design-milk.com"},
            {"visualUrl",
             "http://pbs.twimg.com/profile_images/1765276661/DMLogoTM-carton-icon-facebook-twitter_bigger.jpg"}}};

        svr.Post("/subscriptions", [&data](const httplib::Request &req, httplib::Response &res) {
            REQUIRE_THAT(req.get_header_value("Authorization"), Equals("OAuth " + mock_creds.developer_token));

            auto body = json::parse(req.body);

            REQUIRE_THAT(body["id"], Equals(data[0]["id"]));
            REQUIRE_THAT(body["categories"][0]["id"], Equals(data[0]["categories"][0]["id"]));
            REQUIRE_THAT(body["categories"][0]["label"], Equals(data[0]["categories"][0]["label"]));
            REQUIRE_THAT(body["categories"][1]["id"], Equals(data[0]["categories"][1]["id"]));
            REQUIRE_THAT(body["categories"][1]["label"], Equals(data[0]["categories"][1]["label"]));

            res.status = 200;

            res.set_content(data.dump(), "application/json");
        });

        auto ft = std::async(std::launch::async, [&svr]() { svr.listen("localhost", 1234); });

        AND_GIVEN("a feedly client configured against the mock server") {
            feedly::Client client{mock_creds, mock_server_host};

            WHEN("the client makes an HTTP request") {
                THEN("the client responds with the subscribed feed") {
                    feedly::Feed feed{
                        .title = "Design Milk",
                        .website = "http://feeds.feedburner.com/design-milk",
                    };

                    auto resp = client.subscribe(
                        feed, {{.id = data[0]["categories"][0]["id"], .label = data[0]["categories"][0]["label"]},
                               {.id = data[0]["categories"][1]["id"], .label = data[0]["categories"][1]["label"]}});

                    REQUIRE_THAT(resp.id, Equals(data[0]["id"]));
                    REQUIRE_THAT(resp.title.value(), Equals(data[0]["title"]));
                    REQUIRE_THAT(resp.visual_url, Equals(data[0]["visualUrl"]));
                    REQUIRE_THAT(resp.website, Equals(data[0]["website"]));
                    REQUIRE(resp.updated == data[0]["updated"]);

                    svr.stop();
                    ft.wait();
                }
            }
        }
    }
}

SCENARIO("Make a create category request") {
    GIVEN("A mock HTTP server") {
        httplib::Server svr;

        json data = {{
            {"id", "user/af190c49-0ac8-4f08-9f83-805f1a3bc142/category/c805fcbf-3acf-4302-a97e-d82f9d7c897f"},
            {"label", "Design blogs"},
            {"description", "Influential design blogs"},
            {"cover", "http://imageUrl"},
            {"created", 1367539068016},
        }};

        svr.Post("/collections", [&data](const httplib::Request &req, httplib::Response &res) {
            auto body = json::parse(req.body);

            REQUIRE_THAT(body["label"], Equals("test"));

            res.status = 200;

            res.set_content(data.dump(), "application/json");
        });

        auto ft = std::async(std::launch::async, [&svr]() { svr.listen("localhost", 1234); });

        AND_GIVEN("a feedly client configured against the mock server") {
            feedly::Client client{mock_creds, mock_server_host};

            WHEN("the client makes an HTTP request") {
                THEN("the client responds with the created category") {
                    feedly::Category ctg{
                        .label = "test",
                    };

                    auto resp = client.create_category(ctg);

                    REQUIRE_THAT(resp.label, Equals(data[0]["label"]));
                    REQUIRE_THAT(resp.id, Equals(data[0]["id"]));

                    svr.stop();
                    ft.wait();
                }
            }
        }
    }
}

SCENARIO("List categories") {
    GIVEN("A mock HTTP server") {
        httplib::Server svr;

        json data = {{
            {"id", "user/af190c49-0ac8-4f08-9f83-805f1a3bc142/category/c805fcbf-3acf-4302-a97e-d82f9d7c897f"},
            {"label", "Design blogs"},
        }};

        svr.Get("/categories", [&data](const httplib::Request &req, httplib::Response &res) {
            REQUIRE_THAT(req.get_header_value("Authorization"), Equals("OAuth " + mock_creds.developer_token));

            res.status = 200;

            res.set_content(data.dump(), "application/json");
        });

        auto ft = std::async(std::launch::async, [&svr]() { svr.listen("localhost", 1234); });

        AND_GIVEN("a feedly client configured against the mock server") {
            feedly::Client client{mock_creds, mock_server_host};

            WHEN("the client makes an HTTP request") {
                THEN("the client responds with the a list of categories") {
                    auto resp = client.categories();

                    REQUIRE_THAT(resp[0].label, Equals(data[0]["label"]));

                    svr.stop();
                    ft.wait();
                }
            }
        }
    }
}

SCENARIO("List all account subscriptions") {
    GIVEN("A mock HTTP server") {
        httplib::Server svr;

        json data = {
            {{"id", "feed/http://feeds.feedburner.com/design-milk"},
             {"title", "Design Milk"},
             {"updated", 1367539068016},
             {"website", "http://design-milk.com"},
             {"visualUrl",
              "http://pbs.twimg.com/profile_images/1765276661/DMLogoTM-carton-icon-facebook-twitter_bigger.jpg"}},
            {{"id", "feed/http://5secondrule.typepad.com/my_weblog/atom.xml"},
             {"title", "5 second rule"},
             {"updated", 1367539068016},
             {"website", "http://5secondrule.typepad.com/my_weblog/"},
             {"visualUrl", "http://example.com"}}};

        svr.Get("/subscriptions", [&data](const httplib::Request &req, httplib::Response &res) {
            REQUIRE_THAT(req.get_header_value("Authorization"), Equals("OAuth " + mock_creds.developer_token));

            res.status = 200;

            res.set_content(data.dump(), "application/json");
        });

        auto ft = std::async(std::launch::async, [&svr]() { svr.listen("localhost", 1234); });

        AND_GIVEN("a feedly client configured against the mock server") {
            feedly::Client client{mock_creds, mock_server_host};

            WHEN("the client makes an HTTP request") {
                THEN("the client responds with the a list of subscriptions") {
                    auto resp = client.subscriptions();

                    REQUIRE(resp.size() == 2);

                    REQUIRE_THAT(resp[0].id, Equals(data[0]["id"]));
                    REQUIRE_THAT(resp[0].title.value(), Equals(data[0]["title"]));
                    REQUIRE_THAT(resp[0].visual_url, Equals(data[0]["visualUrl"]));
                    REQUIRE_THAT(resp[0].website, Equals(data[0]["website"]));
                    REQUIRE(resp[0].updated == data[0]["updated"]);

                    REQUIRE_THAT(resp[1].id, Equals(data[1]["id"]));
                    REQUIRE_THAT(resp[1].title.value(), Equals(data[1]["title"]));
                    REQUIRE_THAT(resp[1].visual_url, Equals(data[1]["visualUrl"]));
                    REQUIRE_THAT(resp[1].website, Equals(data[1]["website"]));
                    REQUIRE(resp[1].updated == data[1]["updated"]);

                    svr.stop();
                    ft.wait();
                }
            }
        }
    }
}

SCENARIO("List subscriptions in a category") {
    GIVEN("A mock HTTP server") {
        httplib::Server svr;

        json data = {
            {{"feeds",
              {{{"id", "feed/http://feeds.feedburner.com/design-milk"},
                {"title", "Design Milk"},
                {"updated", 1367539068016},
                {"website", "http://design-milk.com"},
                {"visualUrl",
                 "http://pbs.twimg.com/profile_images/1765276661/DMLogoTM-carton-icon-facebook-twitter_bigger.jpg"}},
               {{"id", "feed/http://5secondrule.typepad.com/my_weblog/atom.xml"},
                {"title", "5 second rule"},
                {"updated", 1367539068016},
                {"website", "http://5secondrule.typepad.com/my_weblog/"},
                {"visualUrl", "http://example.com"}}}}}};

        svr.Get("/collections/categoryID", [&data](const httplib::Request &req, httplib::Response &res) {
            REQUIRE_THAT(req.get_header_value("Authorization"), Equals("OAuth " + mock_creds.developer_token));

            res.status = 200;

            res.set_content(data.dump(), "application/json");
        });

        auto ft = std::async(std::launch::async, [&svr]() { svr.listen("localhost", 1234); });

        AND_GIVEN("a feedly client configured against the mock server") {
            feedly::Client client{mock_creds, mock_server_host};

            WHEN("the client makes an HTTP request") {
                THEN("the client responds with the a list of subscriptions") {
                    auto resp = client.subscriptions("categoryID");

                    REQUIRE(resp.size() == 2);

                    auto feeds = data[0]["feeds"];

                    REQUIRE_THAT(resp[0].id, Equals(feeds[0]["id"]));
                    REQUIRE_THAT(resp[0].title.value(), Equals(feeds[0]["title"]));
                    REQUIRE_THAT(resp[0].visual_url, Equals(feeds[0]["visualUrl"]));
                    REQUIRE_THAT(resp[0].website, Equals(feeds[0]["website"]));
                    REQUIRE(resp[0].updated == feeds[0]["updated"]);

                    REQUIRE_THAT(resp[1].id, Equals(feeds[1]["id"]));
                    REQUIRE_THAT(resp[1].title.value(), Equals(feeds[1]["title"]));
                    REQUIRE_THAT(resp[1].visual_url, Equals(feeds[1]["visualUrl"]));
                    REQUIRE_THAT(resp[1].website, Equals(feeds[1]["website"]));
                    REQUIRE(resp[1].updated == feeds[1]["updated"]);

                    svr.stop();
                    ft.wait();
                }
            }
        }
    }
}
