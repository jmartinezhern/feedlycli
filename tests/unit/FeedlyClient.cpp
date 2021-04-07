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

feedly::DeveloperTokenCredentials mock_creds{
    .developer_token = "developer_token",
    .user_id = "user-id",
};

std::string mock_server_host{"http://localhost:1234"};

using Catch::Matchers::Equals;
using nlohmann::json;

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