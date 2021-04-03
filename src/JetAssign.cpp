#include <algorithm>
#include <limits>
#include <string>
#include <array>
#include <vector>
#include <regex>
#include <iostream>
#include <iomanip>

#include <climits>

using std::size_t;

// array
using std::array;

// iostream
using std::cin;
using std::cout;
using std::endl;
using std::istream;

// optional
using std::optional;

// string
using std::string;

// vector
using std::vector;

#define JET_ROW_LENGTH 13

#define JET_COLUMN_LENGTH 6

#define STRINGIFY(expression) #expression

#define STRINGIFY_VALUE(value) STRINGIFY(value)

/**
 * Utility functions for std::string.
 **/
namespace stringutil
{
    /**
     * Removes the leading and trailing whitespaces of a string.
     * 
     * @param input The string to be trimmed.
     **/
    string trim(const string &input);

    /**
     * Removes the leading whitespaces of a string.
     * 
     * @param input The string to be trimmed.
     **/
    string trim_start(const string &input);

    /**
     * Removes the trailing whitespaces of a string.
     * 
     * @param input The string to be trimmed.
     **/
    string trim_end(const string &input);

    /**
     * Converts the string into uppercased.
     * 
     * @param input The string to be converted.
     **/
    string to_uppercase(string input);

    /**
     * Splits the string into multiple segments by the given separator.
     * 
     * @param input     The string to be split.
     * @param separator The separator to split the string.
     **/
    vector<string> split(const string &input, const string &separator);
}

namespace jetassign
{
    namespace
    {
        const auto kRowLength = JET_ROW_LENGTH;
        const auto kColumnLength = JET_COLUMN_LENGTH;
        const auto kJetSize = kRowLength * kColumnLength;
    }

    /**
     * The core component.
     **/
    namespace core
    {
        class Passenger
        {
            public:
                Passenger(const string &name, const string &passport_id);

                string get_name() const { return name; }

                string get_passport_id() const { return passport_id; }

                bool equals(const Passenger &other) const;

                bool operator ==(const Passenger &other) const { return equals(other); }

                bool operator !=(const Passenger &other) const { return !equals(other); }

            private:
                /**
                 * The name of the passenger.
                 **/
                string name;

                /**
                 * The passport ID of the passenger.
                 **/
                string passport_id;
        };

        class SeatLocation
        {
            public:
                static string row_to_string(size_t row);

                static string column_to_string(size_t column);

                SeatLocation(size_t index);

                SeatLocation(size_t row, size_t column);

                size_t get_row() const { return row; }

                size_t get_column() const { return column; }

                size_t get_index() const;

                bool equals(const SeatLocation &other) const;

                bool operator ==(const SeatLocation &other) const { return equals(other); }

                bool operator !=(const SeatLocation &other) const { return !equals(other); }

                string to_string() const;

            private:
                /**
                 * The row of the seat.
                 **/
                size_t row;

                /**
                 * The column of the seat.
                 **/
                size_t column;
        };

        class SeatingPlan
        {
            public:
                SeatingPlan();

                /**
                 * Determine whether the seat was already occupied by a passenger.
                 * 
                 * @param location The location of the seat.
                 **/
                bool is_occupied(const SeatLocation &location) const;

                /**
                 * Determine whether a seat was already assigned to the passenger.
                 **/
                bool is_assigned(const Passenger &passenger) const;

                /**
                 * Returns the passenger who was assigned to the given seat.
                 * 
                 * @param location The location of the seat.
                 **/
                const optional<Passenger> at(const SeatLocation &location) const;

                optional<SeatLocation> location_of(const Passenger &passenger) const;

                /**
                 * Assign a passenger to a specific seat.
                 * 
                 * @param location  The location of the seat.
                 * @param passenger The passenger to be assigned.
                 **/
                void assign(const SeatLocation &location, const Passenger &passenger);

            private:
                /**
                 * The internal seating plan.
                 **/
                array<array<optional<Passenger>, kColumnLength>, kRowLength> seating_plan;
        };
    }

    /**
     * The exceptions component.
     **/
    namespace exceptions
    {
        using std::invalid_argument;
        using std::runtime_error;

        using core::SeatLocation;

        /**
         * An error that will throw when the requested seat was already occupied by another passenger.
         **/
        class SeatOccupiedError : public runtime_error
        {
            public:
                SeatOccupiedError(const SeatLocation &location)
                    : runtime_error("The requested seat was already occupied by another passenger."),
                      location { location } {};

                /**
                 * Returns the location of the occupied seat.
                 **/
                const SeatLocation get_location() const { return location; }

            private:
                SeatLocation location;
        };

        class InvalidInputError : public invalid_argument
        {
            using invalid_argument::invalid_argument;
        };

        class EmptyInputError : public InvalidInputError
        {
            using InvalidInputError::InvalidInputError;
        };

        class MalformedInputError : public InvalidInputError
        {
            using InvalidInputError::InvalidInputError;
        };
    }

    /**
     * The input component.
     **/
    namespace input
    {
        using core::Passenger;
        using core::SeatLocation;

        class CompactAssignment
        {
            public:
                CompactAssignment(const string &passenger_name, const string &passport_id, const SeatLocation &seat_location);

                Passenger get_passenger() const { return passenger; }

                SeatLocation get_location() const { return location; }

            private:
                Passenger passenger;
                SeatLocation location;
        };

        /**
         * Read the user's input until an EOL character was received.
         **/
        string read_line();

        long get_menu_option(long max);

        long get_menu_option(long min, long max);

        string get_passenger_name();

        string get_passport_id();

        SeatLocation get_seat_location();

        Passenger get_passenger();

        /**
         * The input parsers component.
         **/
        namespace parsers
        {

            long parse_menu_option(const string &input);

            string parse_passenger_name(const string &input);

            string parse_passport_id(const string &input);

            SeatLocation parse_seat_location(const string &input);

            CompactAssignment parse_compact_assignment(const string &input);
        }
    }

    auto seating_plan = core::SeatingPlan();
}

#ifndef _TEST
int main(int argc, const char* argv[])
{
    // jetassign::input::get_passenger_name();
    // jetassign::input::get_passport_id();
    // jetassign::input::get_seat_location();

    jetassign::core::Passenger passenger("J", "H");
    jetassign::core::SeatLocation location(0, 0);

    jetassign::seating_plan.assign(location, passenger);
    cout << jetassign::seating_plan.location_of(passenger).value().to_string() << endl;

    return 0;
}
#endif

namespace jetassign::core
{
    using std::range_error;

    namespace
    {
        using std::to_string;

        const auto kSeatLocationRowRangeErrorMessage = "The range of the row must between 0 (inclusive) and " STRINGIFY_VALUE(JET_ROW_LENGTH) " (exclusive).";

        const auto kSeatLocationColumnRangeErrorMessage = "The range of the column must between 0 (inclusive) and " STRINGIFY_VALUE(JET_COLUMN_LENGTH) " (exclusive).";
    }

    SeatingPlan::SeatingPlan() {}

    bool SeatingPlan::is_occupied(const SeatLocation &location) const
    {
        return seating_plan.at(location.get_row()).at(location.get_column()).has_value();
    }

    bool SeatingPlan::is_assigned(const Passenger &passenger) const
    {
        return this->location_of(passenger).has_value();
    }

    const optional<Passenger> SeatingPlan::at(const SeatLocation &location) const
    {
        return seating_plan.at(location.get_row()).at(location.get_column());
    }

    optional<SeatLocation> SeatingPlan::location_of(const Passenger &passenger) const
    {
        for (auto r = 0; r < kRowLength; r++)
        {
            for (auto c = 0; c < kColumnLength; c++)
            {
                if (seating_plan.at(r).at(c) == passenger)
                {
                    return SeatLocation(r, c);
                }
            }
        }

        return std::nullopt;
    }

    void SeatingPlan::assign(const SeatLocation &location, const Passenger &passenger)
    {
        if (this->is_occupied(location))
        {
            throw exceptions::SeatOccupiedError(location);
        }

        seating_plan.at(location.get_row()).at(location.get_column()) = passenger;
    }

    Passenger::Passenger(const string &name, const string &passport_id)
        : name { name }, passport_id { passport_id } {}

    bool Passenger::equals(const Passenger &other) const
    {
        return ((name == other.name) && (passport_id == other.passport_id));
    }

    string SeatLocation::row_to_string(size_t row)
    {
        if (row >= kRowLength)
        {
            throw range_error(kSeatLocationRowRangeErrorMessage);
        }

        return std::to_string(row + 1);
    }

    string SeatLocation::column_to_string(size_t column)
    {
        if (column >= kColumnLength)
        {
            throw range_error(kSeatLocationColumnRangeErrorMessage);
        }

        return string(1, (char) ('A' + column));
    }

    SeatLocation::SeatLocation(size_t index)
        : SeatLocation(index / kColumnLength, index % kColumnLength) {}

    SeatLocation::SeatLocation(size_t row, size_t column)
        : row { row }, column { column }
    {
        if (row >= kRowLength)
        {
            throw range_error(kSeatLocationRowRangeErrorMessage);
        }
        else if (column >= kColumnLength)
        {
            throw range_error(kSeatLocationColumnRangeErrorMessage);
        }
    }

    size_t SeatLocation::get_index() const
    {
        return ((row * kColumnLength) + column);
    }

    bool SeatLocation::equals(const SeatLocation &other) const
    {
        return ((row == other.row) && (column == other.column));
    }

    string SeatLocation::to_string() const
    {
        return (row_to_string(row) + column_to_string(column));
    }
}

namespace jetassign::exceptions
{

}

namespace jetassign::input
{
    using exceptions::InvalidInputError;

    string read_line()
    {
        string line;
        std::getline(cin, line);

        return line;
    }

    long get_menu_option(long max)
    {
        return get_menu_option(1, max);
    }

    long get_menu_option(long min, long max)
    {
        while (true)
        {
            cout << "Option (" << min << "-" << max << "): ";
            auto input = read_line();

            try
            {
                const auto selection = parsers::parse_menu_option(input);
                if ((selection < min) || (selection > max))
                {
                    std::cerr << "    Error: The option selection must between " << min << " and " << max << " (inclusive)." << endl;
                    continue;
                }

                return selection;
            }
            catch(const InvalidInputError &e)
            {
                std::cerr << "    Error: " << e.what() << endl;;
            }  
        }
    }

    Passenger get_passenger()
    {
        auto passenger_name = get_passenger_name();
        auto passport_id = get_passport_id();

        return Passenger(passenger_name, passport_id);
    }

    string get_passenger_name()
    {
        while (true)
        {
            cout << "Passenger Name: ";
            auto input = read_line();

            try
            {
                return parsers::parse_passenger_name(input);
            }
            catch(const InvalidInputError &e)
            {
                std::cerr << "    Error: " << e.what() << endl;;
            }  
        }
    }

    string get_passport_id()
    {
        while (true)
        {
            cout << "Passport ID: ";
            auto input = read_line();

            try
            {
                return parsers::parse_passport_id(input);
            }
            catch(const InvalidInputError &e)
            {
                std::cerr << "    Error: " << e.what() << endl;;
            } 
        }
    }

    SeatLocation get_seat_location()
    {
        while (true)
        {
            cout << "Seat Location: ";
            auto input = read_line();

            try
            {
                return parsers::parse_seat_location(input);
            }
            catch(const InvalidInputError &e)
            {
                std::cerr << "    Error: " << e.what() << endl;;
            } 
        }
    }

    CompactAssignment::CompactAssignment(const string &passenger_name, const string &passport_id, const SeatLocation &seat_location)
        : passenger(passenger_name, passport_id), location { seat_location } {};

    namespace parsers
    {
        using std::regex_match;
        using std::smatch;

        using exceptions::EmptyInputError;
        using exceptions::MalformedInputError;

        namespace
        {
            using std::regex;

            const regex kMenuOptionPattern(R"((\d+))");

            const regex kPassportIdPattern("([0-9A-Z]+)", regex::icase);

            const regex kSeatLocationPattern("(1[0-3]|[1-9])([A-F])");

            const auto kCompactAssignmentSeparator = "/";
        }

        long parse_menu_option(const string &input)
        {
            const auto selection = stringutil::trim(input);
            if (selection.empty())
            {
                throw EmptyInputError("The option selection must not be empty.");
            }

            if (!regex_match(selection, kMenuOptionPattern))
            {
                throw MalformedInputError("Only numeric characters were allowed.");
            }

            return std::stol(selection);
        }

        string parse_passenger_name(const string &input)
        {
            const auto passenger_name = stringutil::trim(input);
            if (passenger_name.empty())
            {
                throw EmptyInputError("The passenger's name must not be empty.");
            }

            return passenger_name;
        }

        string parse_passport_id(const string &input)
        {
            auto passport_id = stringutil::trim(input);
            if (passport_id.empty())
            {
                throw EmptyInputError("The passport ID must not be empty.");
            }

            if (!regex_match(passport_id, kPassportIdPattern))
            {
                throw MalformedInputError("Only alphanumeric characters were allowed.");
            }

            return passport_id;
        }

        SeatLocation parse_seat_location(const string &input)
        {
            auto seat_location = stringutil::to_uppercase(stringutil::trim(input));
            if (seat_location.empty())
            {
                throw EmptyInputError("The seat location must not be empty.");
            }

            std::smatch match_result;
            if (!regex_match(seat_location, match_result, kSeatLocationPattern))
            {
                throw MalformedInputError(R"(The seat location must be formatted as the row (1-13) followed by the column (A-F), e.g. "10D".)");
            }

            auto row = std::stoi(match_result.str(1)) - 1;
            auto column = match_result.str(2).at(0) - 'A';
            
            return SeatLocation(row, column);
        }

        CompactAssignment parse_compact_assignment(const string &input)
        {
            const auto input_segments = stringutil::split(stringutil::trim(input), kCompactAssignmentSeparator);
            if (input_segments.size() != 3)
            {
                throw exceptions::MalformedInputError(R"(The assignment entry should be formatted as "<Name>/<Passport ID>/<Seat Location>".)");
            }

            auto passenger_name = parse_passenger_name(input_segments.at(0));
            auto passport_id = parse_passport_id(input_segments.at(1));
            SeatLocation seat_location = parse_seat_location(input_segments.at(2));

            return CompactAssignment(passenger_name, passport_id, seat_location);
        }
    }
}

namespace stringutil
{
    namespace
    {
        const auto kWhitespace = "\t\n\v\f\r\x20\xA0";
    }

    string trim(const string &input)
    {
        return trim_end(trim_start(input));
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
