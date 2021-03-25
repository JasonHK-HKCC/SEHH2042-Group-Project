#include <algorithm>
#include <limits>
#include <string>
#include <array>
#include <vector>
#include <regex>
#include <iostream>
#include <functional>

using std::size_t;

// string
using std::string;

// vector
using std::vector;

// array
using std::array;

// regex
using std::regex;
using std::regex_match;
using std::regex_search;

// iostream
using std::cin;
using std::cout;
using std::endl;
using std::istream;

namespace stringutil
{
    namespace
    {
        const auto kWhitespace = "\t\n\v\f\r\x20\xA0";
    }

    string trim_start(const string &input)
    {
        const auto index = input.find_first_not_of(kWhitespace);
        return ((index == string::npos) ? "" : input.substr(index));
    }

    string trim_end(const string &input)
    {
        const auto index = input.find_last_not_of(kWhitespace);
        return ((index == string::npos) ? "" : input.substr(0, index + 1));
    }

    string trim(const string &input)
    {
        return trim_end(trim_start(input));
    }

    string to_uppercase(string input)
    {
        string output = input;
        std::transform(input.begin(), input.end(), output.begin(), [](unsigned char c) { return std::toupper(c); });

        return output;
    }

    vector<string> split(const string &input, const string &separator)
    {
        vector<string> segments;

        if (separator.empty())
        {
            segments.push_back(input);
        }
        else
        {
            size_t start_pos = 0;
            do
            {
                auto segment = input.substr(start_pos, input.find(separator, start_pos) - start_pos);

                segments.push_back(segment);
                start_pos += segment.length() + separator.length();
            }
            while (start_pos <= input.length());
        }

        return segments;
    }
}

namespace jetassign
{
    namespace
    {
        const regex kPassportIdPattern("([0-9A-Z]+)", regex::icase);

        const regex kSeatLocationPattern("(1[0-3]|[1-9])([A-F])");

        const auto kCompactAssignmentSeparator = "/";
    }

    struct SeatLocation
    {
        size_t row;
        size_t column;

        // SeatLocation& operator =(SeatLocation &other)
        // {
        //     row    = other.row;
        //     column = other.column;
        //
        //     return other;
        // }

        inline bool equals(const SeatLocation &other) const
        {
            return ((row == other.row) && (column == other.column));
        }

        inline string to_string() const
        {
            return (std::to_string(row + 1) + ((char) ('A' + column)));
        }

        inline bool operator ==(const SeatLocation &other) const
        {
            return equals(other);
        }

        inline bool operator !=(const SeatLocation &other) const
        {
            return !equals(other);
        }

        inline operator string() const
        {
            return (std::to_string(row + 1) + ((char) ('A' + column)));
        }
    };

    bool is_passport_id(string input)
    {
        return regex_search(input, kPassportIdPattern);
    }

    bool parse_passenger_name(string input, string &passenger_name)
    {
        const auto trimmed_input = stringutil::trim(input);
        if (trimmed_input.empty()) { return false; }

        passenger_name = trimmed_input;
        return true;
    }

    bool parse_passport_id(const string &input, string &passport_id)
    {
        auto trimmed_input = stringutil::trim(input);
        if (!regex_match(trimmed_input, kPassportIdPattern))
        {
            return false;
        }

        passport_id = trimmed_input;
        return true;
    }

    bool parse_seat_location(const string &input, size_t &row, size_t &column)
    {
        auto trimmed_input = stringutil::to_uppercase(stringutil::trim(input));

        std::smatch match_result;
        if (!regex_match(input, match_result, kSeatLocationPattern))
        {
            return false;
        }

        row = std::stoi(match_result.str(1)) - 1;
        column = match_result.str(2).at(0) - 'A';
        
        return true;
    }

    bool parse_seat_location(const string &input, SeatLocation &location)
    {
        return parse_seat_location(input, location.row, location.column);
    }

    bool parse_compact_assignment(const string &input, string &name, string &passport_id, SeatLocation &seat_location)
    {
        const auto input_segments = stringutil::split(stringutil::trim(input), kCompactAssignmentSeparator);
        if (input_segments.size() != 3) { return false; }

        string parsed_name;
        if (!parse_passenger_name(input_segments.at(0), parsed_name))
        {
            return false;
        }

        string parsed_passport_id;
        if (!parse_passport_id(input_segments.at(1), parsed_passport_id))
        {
            return false;
        }

        SeatLocation parsed_seat_location;
        if (!parse_seat_location(input_segments.at(2), parsed_seat_location))
        {
            return false;
        }

        name          = parsed_name;
        passport_id   = parsed_passport_id;
        seat_location = parsed_seat_location;

        return true;
    }
}

#ifndef _TEST
int main(int argc, const char* argv[])
{
    return 0;
}
#endif
