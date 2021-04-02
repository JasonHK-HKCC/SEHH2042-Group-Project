#include "catch.hpp"

#include "JetAssign.cpp"

namespace
{
    using std::make_tuple;

    namespace Matchers = Catch::Matchers;
    namespace Generators = Catch::Generators;

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
                REQUIRE_THAT(split("", kSeparator), Matchers::Equals(vector<string> { "" }));
            }

            AND_WHEN("the input is a string that doesn't contain any separators")
            {
                REQUIRE_THAT(split("a", kSeparator), Matchers::Equals(vector<string> { "a" }));
            }
        }

        GIVEN("the separator is a single-character string")
        {
            const string kSeparator = ",";

            AND_WHEN("the input is an empty string")
            {
                REQUIRE_THAT(split("", kSeparator), Matchers::Equals(vector<string> { "" }));
            }

            AND_WHEN("the input is a string that doesn't contain any separators")
            {
                REQUIRE_THAT(split("a", kSeparator), Matchers::Equals(vector<string> { "a" }));
            }

            AND_WHEN("the input is a string that contains separators only")
            {
                REQUIRE_THAT(split(",,", kSeparator), Matchers::Equals(vector<string> { "", "", "" }));
            }

            AND_WHEN("the input is a string that contains separators and other characters")
            {
                REQUIRE_THAT(split("a,b,c", kSeparator), Matchers::Equals(vector<string> { "a", "b", "c" }));
            }
        }

        GIVEN("the separator is a multi-character string")
        {
            const string kSeparator = "->";

            AND_WHEN("the input is an empty string")
            {
                REQUIRE_THAT(split("", kSeparator), Matchers::Equals(vector<string> { "" }));
            }

            AND_WHEN("the input is a string that doesn't contain any separators")
            {
                REQUIRE_THAT(split("a", kSeparator), Matchers::Equals(vector<string> { "a" }));
            }

            AND_WHEN("the input is a string that contains separators only")
            {
                REQUIRE_THAT(split("->->", kSeparator), Matchers::Equals(vector<string> { "", "", "" }));
            }

            AND_WHEN("the input is a string that contains separators and other characters")
            {
                REQUIRE_THAT(split("a->b->c", kSeparator), Matchers::Equals(vector<string> { "a", "b", "c" }));
            }
        }
    }

    // TEST_CASE("jetassign::is_passport_id")
    // {
    //     using jetassign::is_passport_id;

    //     GIVEN("return true")
    //     {
    //         WHEN("the payload was a valid passport ID")
    //         {
    //             REQUIRE(is_passport_id("HK12345678A"));

    //             REQUIRE(is_passport_id("UPPERCASE"));
    //             REQUIRE(is_passport_id("lowercase"));
    //             REQUIRE(is_passport_id("MiXeDcAsE"));
    //         }
    //     }

    //     GIVEN("return false")
    //     {
    //         WHEN("the payload was empty")
    //         {
    //             REQUIRE_FALSE(is_passport_id(""));
    //         }

    //         WHEN("the payload contain whitespaces only")
    //         {
    //             REQUIRE_FALSE(is_passport_id(" "));
    //         }
    //     }
    // }

    // TEST_CASE("jetassign::parse_passenger_name")
    // {
    //     using jetassign::parse_passenger_name;

    //     WHEN("the input was an empty string")
    //     {
    //         string passenger_name;
    //         REQUIRE_FALSE(parse_passenger_name("", passenger_name));
    //         REQUIRE(passenger_name == "");
    //     }

    //     WHEN("the input was a string that contains a passenger's name")
    //     {
    //         string test_input;
    //         string expected_passenger_name;

    //         std::tie(test_input, expected_passenger_name) = GENERATE(
    //             Generators::table<string, string>(
    //                 {
    //                     make_tuple("Chan Tai Man", "Chan Tai Man"),
    //                     make_tuple("foo bar", "foo bar"),
    //                 }));

    //         {
    //             string passenger_name;
    //             REQUIRE(parse_passenger_name("Chan Tai Man", passenger_name));
    //             REQUIRE(passenger_name == "Chan Tai Man");
    //         }
    //     }
    // }

    // TEST_CASE("jetassign::parse_passport_id")
    // {
    //     using jetassign::parse_passport_id;
        
    //     WHEN("the input was not a valid passport ID")
    //     {
    //         string test_input = GENERATE(
    //             "",
    //             u"／人◕ ‿‿ ◕人＼");

    //         {
    //             string passport_id;
                
    //             THEN("should return false")
    //             {
    //                 REQUIRE_FALSE(parse_passport_id(test_input, passport_id));

    //                 AND_THEN("passport_id should not be altered")
    //                 {
    //                     REQUIRE(passport_id.empty());
    //                 }
    //             }
    //         }
    //     }

    //     WHEN("the input was a valid passport ID")
    //     {
    //         string test_input;
    //         string expected_passport_id;

    //         std::tie(test_input, expected_passport_id) = GENERATE(
    //             Generators::table<string, string>(
    //                 {
    //                     make_tuple("ABC", "ABC"),
    //                     make_tuple("123", "123"),
    //                     make_tuple("ABC123", "ABC123"),
    //                     make_tuple("123ABC", "123ABC"),
    //                 }));

    //         {
    //             string passport_id;
                
    //             THEN("should return true")
    //             {
    //                 REQUIRE(parse_passport_id(test_input, passport_id));

    //                 AND_THEN("passport_id should be updated with the parsed passport ID")
    //                 {
    //                     REQUIRE(passport_id == expected_passport_id);
    //                 }
    //             }
    //         }
    //     }
    // }

    // TEST_CASE("jetassign::parse_seat_location")
    // {
    //     using jetassign::SeatLocation;
    //     using jetassign::parse_seat_location;

    //     GIVEN("return true")
    //     {
    //         size_t row;
    //         size_t column;

    //         REQUIRE(parse_seat_location("10D", row, column));
    //         REQUIRE(row == 9);
    //         REQUIRE(column == 3);

    //         REQUIRE(parse_seat_location("1A", row, column));
    //         REQUIRE(row == 0);
    //         REQUIRE(column == 0);
    //     }
    // }

    // TEST_CASE("jetassign::parse_compact_assignment")
    // {
    //     using jetassign::SeatLocation;
    //     using jetassign::parse_compact_assignment;

    //     GIVEN("the input format was not correct")
    //     {
    //         #define MACRO_WHEN(desc, input) \
    //             WHEN(desc) \
    //             { \
    //                 string       passenger_name; \
    //                 string       passport_id; \
    //                 SeatLocation seat_location; \
    //                 \
    //                 THEN("it should return false") \
    //                 { \
    //                     REQUIRE_FALSE(parse_compact_assignment(input, passenger_name, passport_id, seat_location)); \
    //                 } \
    //             }

    //         MACRO_WHEN("the number of input segments was not 3", "")
    //         MACRO_WHEN("the number of input segments was not 3", "/")
    //         MACRO_WHEN("the number of input segments was not 3", "///")

    //         #undef MACRO_WHEN
    //     }
    // }

    // TEST_CASE("jetassign::SeatLocation::operator ==")
    // {
    //     using jetassign::SeatLocation;

    //     SeatLocation location { 0, 0 };
    //     REQUIRE(location == SeatLocation { 0, 0 });
    //     REQUIRE_FALSE(location == SeatLocation { 12, 5 });
    // }

    // TEST_CASE("jetassign::SeatLocation::operator !=")
    // {
    //     using jetassign::SeatLocation;

    //     SeatLocation location { 12, 5 };
    //     REQUIRE(location != SeatLocation { 0, 0 });
    //     REQUIRE_FALSE(location != SeatLocation { 12, 5 });
    // }

    // TEST_CASE("jetassign::SeatLocation::to_string")
    // {
    //     using jetassign::SeatLocation;

    //     SeatLocation location { 9, 3 };
    //     REQUIRE(location.to_string() == "10D");
    // }
}
