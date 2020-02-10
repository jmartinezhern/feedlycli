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

std::string get_access_token() { return std::getenv("FEEDLY_ACCESS_TOKEN"); }

SCENARIO("Get Categories") {
    GIVEN("An authenticate client") {
        feedly::Client client{
            {.developer_token = get_access_token(), .user_id = "b809d8d9-6784-41b7-9f20-d4fc4307e175"}};

        AND_GIVEN("an existing category") {
            auto ctg = client.create_category({.label = "test" + std::to_string(random_int_in_range(0, 999999999))});

            WHEN("the client gets categories") {
                auto ctgs = client.categories();

                THEN("it returns a list of categories containing the created category") {
                    REQUIRE(ctgs.size() > 0);

                    REQUIRE_THAT(ctgs, VectorContains(ctg));
                }
            }
        }
    }
}
