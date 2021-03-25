#include "catch.hpp"

#include "JetAssign.cpp"

using Catch::Matchers::Equals;

TEST_CASE("stringutil::to_uppercase")
{
    using stringutil::to_uppercase;

    WHEN(R"(input is an empty string)")
    {
        string input = "";
        REQUIRE(to_uppercase(input) == "");
        REQUIRE(input == "");
    }

    WHEN(R"(input is a string)")
    {
        string input = "UPPERCASE";
        REQUIRE(to_uppercase(input) == "UPPERCASE");
        REQUIRE(input == "UPPERCASE");
    }

    WHEN(R"(input is a string)")
    {
        string input = "lowercase";
        REQUIRE(to_uppercase(input) == "LOWERCASE");
        REQUIRE(input == "lowercase");
    }
}

TEST_CASE("stringutil::split")
{
    using stringutil::split;

    GIVEN("the separator is an empty string")
    {
        const string kSeparator = "";

        AND_WHEN("the input is an empty string")
        {
            REQUIRE_THAT(split("", kSeparator), Equals(vector<string> { "" }));
        }

        AND_WHEN("the input is a string that doesn't contain any separators")
        {
            REQUIRE_THAT(split("a", kSeparator), Equals(vector<string> { "a" }));
        }
    }

    GIVEN("the separator is a single-character string")
    {
        const string kSeparator = ",";

        AND_WHEN("the input is an empty string")
        {
            REQUIRE_THAT(split("", kSeparator), Equals(vector<string> { "" }));
        }

        AND_WHEN("the input is a string that doesn't contain any separators")
        {
            REQUIRE_THAT(split("a", kSeparator), Equals(vector<string> { "a" }));
        }

        AND_WHEN("the input is a string that contains separators only")
        {
            REQUIRE_THAT(split(",,", kSeparator), Equals(vector<string> { "", "", "" }));
        }

        AND_WHEN("the input is a string that contains separators and other characters")
        {
            REQUIRE_THAT(split("a,b,c", kSeparator), Equals(vector<string> { "a", "b", "c" }));
        }
    }

    GIVEN("the separator is a multi-character string")
    {
        const string kSeparator = "->";

        AND_WHEN("the input is an empty string")
        {
            REQUIRE_THAT(split("", kSeparator), Equals(vector<string> { "" }));
        }

        AND_WHEN("the input is a string that doesn't contain any separators")
        {
            REQUIRE_THAT(split("a", kSeparator), Equals(vector<string> { "a" }));
        }

        AND_WHEN("the input is a string that contains separators only")
        {
            REQUIRE_THAT(split("->->", kSeparator), Equals(vector<string> { "", "", "" }));
        }

        AND_WHEN("the input is a string that contains separators and other characters")
        {
            REQUIRE_THAT(split("a->b->c", kSeparator), Equals(vector<string> { "a", "b", "c" }));
        }
    }
}

TEST_CASE("jetassign::is_passport_id")
{
    using jetassign::is_passport_id;

    GIVEN("return true")
    {
        WHEN("the payload was a valid passport ID")
        {
            REQUIRE(is_passport_id("HK12345678A"));

            REQUIRE(is_passport_id("UPPERCASE"));
            REQUIRE(is_passport_id("lowercase"));
            REQUIRE(is_passport_id("MiXeDcAsE"));
        }
    }

    GIVEN("return false")
    {
        WHEN("the payload was empty")
        {
            REQUIRE_FALSE(is_passport_id(""));
        }

        WHEN("the payload contain whitespaces only")
        {
            REQUIRE_FALSE(is_passport_id(" "));
        }
    }
}

    TEST_CASE("jetassign::parse_passenger_name")
    {
        using jetassign::parse_passenger_name;

        WHEN("the input was an empty string")
        {
            string name;
            REQUIRE_FALSE(parse_passenger_name("", name));
            REQUIRE(name == "");
        }

        WHEN("the input was a string that contains a passenger's name")
        {
            string name;
            REQUIRE(parse_passenger_name("Chan Tai Man", name));
            REQUIRE(name == "Chan Tai Man");
        }
    }

TEST_CASE("jetassign::parse_seat_location")
{
    using jetassign::parse_seat_location;

    GIVEN("return true")
    {
        size_t row;
        size_t column;

        REQUIRE(parse_seat_location("10D", row, column));
        REQUIRE(row == 9);
        REQUIRE(column == 3);

        REQUIRE(parse_seat_location("1A", row, column));
        REQUIRE(row == 0);
        REQUIRE(column == 0);
    }
}

TEST_CASE("jetassign::SeatLocation")
{
    using jetassign::SeatLocation;

    SeatLocation location_a { 0, 0 };
    SeatLocation location_b { 9, 3 };

    location_a = location_b;
    REQUIRE(location_a.row == location_b.row);
    REQUIRE(location_a.column == location_b.column);
}

TEST_CASE("jetassign::SeatLocation::to_string")
{
    using jetassign::SeatLocation;

    SeatLocation location { 9, 3 };
    REQUIRE(location.to_string() == "10D");
}
