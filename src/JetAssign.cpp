#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <set>
#include <string>
#include <vector>

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

/**
 * The internals of JetAssign
 **/
namespace jetassign
{
    /**
     * The core component.
     **/
    namespace core
    {
        /**
         * Represents a passenger.
         **/
        class Passenger
        {
            public:
                /**
                 * Initialize a passenger with its information.
                 * 
                 * @param name        The name of the passenger.
                 * @param passport_id The passport ID of the passenger.
                 **/
                Passenger(const string &name, const string &passport_id);

                /**
                 * Returns the name of the passenger.
                 **/
                string get_name() const { return name; }

                /**
                 * Returns the passport ID of the passenger.
                 **/
                string get_passport_id() const { return passport_id; }

                /**
                 * Determine whether two instances represent the same passenger.
                 * 
                 * @param other The other instance.
                 **/
                bool equals(const Passenger &other) const;

                /**
                 * Determine whether two instances represent the same passenger.
                 * 
                 * @param other The other instance.
                 **/
                bool operator ==(const Passenger &other) const { return equals(other); }

                /**
                 * Determine whether two instances represent different passengers.
                 * 
                 * @param other The other instance.
                 **/
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

        /**
         * Represents the location of a seat.
         **/
        class SeatLocation
        {
            public:
                static string row_to_string(size_t row);

                static string column_to_string(size_t column);

                /**
                 * Initialize a seat location with its position.
                 * 
                 * @param row    The row location of the seat.
                 * @param column The column location of the seat.
                 **/
                SeatLocation(size_t row, size_t column);

                /**
                 * Returns the row location of the seat.
                 **/
                size_t get_row() const { return row; }

                /**
                 * Returns the column location of the seat.
                 **/
                size_t get_column() const { return column; }

                /**
                 * Determine whether two instances represent the same seat location.
                 * 
                 * @param other The other instance.
                 **/
                bool equals(const SeatLocation &other) const;

                /**
                 * Determine whether two instances represent the same seat location.
                 * 
                 * @param other The other instance.
                 **/
                bool operator ==(const SeatLocation &other) const { return equals(other); }

                /**
                 * Determine whether two instances represent different seat location.
                 * 
                 * @param other The other instance.
                 **/
                bool operator !=(const SeatLocation &other) const { return !equals(other); }

                string to_string() const;

            private:
                /**
                 * The row location of the seat.
                 **/
                size_t row;

                /**
                 * The column location of the seat.
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

                /**
                 * Remove a passenger at the specific seat from the seating plan.
                 * 
                 * @param location The location of the seat.
                 **/
                void remove(const SeatLocation &location);

                // TODO: Added remove
                // void remove(const Passenger &passenger);

            private:
                /**
                 * The internal seating plan.
                 **/
                array<array<optional<Passenger>, JET_COLUMN_LENGTH>, JET_ROW_LENGTH> seating_plan;
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

                bool equals(const CompactAssignment &other) const;

                bool operator ==(const CompactAssignment &other) const { return equals(other); }

                bool operator !=(const CompactAssignment &other) const { return !equals(other); }

                string to_string() const;

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

        Passenger get_passenger();

        SeatLocation get_seat_location();

        vector<CompactAssignment> get_compact_assignments();

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

/**
 * R1: Add an assignment
 **/
void add_an_assignment();

/**
 * R2: Delete an assignment
 **/
void delete_an_assignment();

/**
 * R3: Add assignments in batch
 **/
void add_assignments_in_batch();

/**
 * R4: Show latest seating plan
 **/
void show_latest_seating_plan();

/**
 * R5: Show details
 **/
void show_details(long selection);

#ifndef _TEST
int main(int argc, const char* argv[])
{
    long selection;
    while ((selection = jetassign::input::get_menu_option(6)) != 6)
    {
        switch (selection)
        {
            case 1:
                add_an_assignment();
                break;
            case 2:
                delete_an_assignment();
                break;
            case 3:
                add_assignments_in_batch();
                break;
            case 4:
                show_latest_seating_plan();
                break;
            case 5:
            {
                long details_selection;
                while ((details_selection = jetassign::input::get_menu_option(3)) != 3)
                {
                    show_details(details_selection);
                }

                break;
            }
        }
    }

    return 0;
}
#endif

void add_an_assignment()
{

}

void delete_an_assignment()
{

}

void add_assignments_in_batch()
{

}

void show_latest_seating_plan()
{

}

void show_details(long selection)
{

}

namespace jetassign::core
{
    using std::range_error;

    namespace
    {
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
        for (auto r = 0; r < JET_ROW_LENGTH; r++)
        {
            for (auto c = 0; c < JET_COLUMN_LENGTH; c++)
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

    void SeatingPlan::remove(const SeatLocation &location)
    {
        if (this->is_occupied(location))
        {
            cout << "DEBUG: this->is_occupied(location) => true" << endl; 
            seating_plan.at(location.get_row()).at(location.get_row()).reset();
        }
    }

    Passenger::Passenger(const string &name, const string &passport_id)
        : name { name }, passport_id { passport_id } {}

    bool Passenger::equals(const Passenger &other) const
    {
        return ((name == other.name) && (passport_id == other.passport_id));
    }

    string SeatLocation::row_to_string(size_t row)
    {
        if (row >= JET_ROW_LENGTH)
        {
            throw range_error(kSeatLocationRowRangeErrorMessage);
        }

        return std::to_string(row + 1);
    }

    string SeatLocation::column_to_string(size_t column)
    {
        if (column >= JET_COLUMN_LENGTH)
        {
            throw range_error(kSeatLocationColumnRangeErrorMessage);
        }

        return string(1, (char) ('A' + column));
    }

    SeatLocation::SeatLocation(size_t row, size_t column)
        : row { row }, column { column }
    {
        if (row >= JET_ROW_LENGTH)
        {
            throw range_error(kSeatLocationRowRangeErrorMessage);
        }
        else if (column >= JET_COLUMN_LENGTH)
        {
            throw range_error(kSeatLocationColumnRangeErrorMessage);
        }
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

    Passenger get_passenger()
    {
        auto passenger_name = get_passenger_name();
        auto passport_id = get_passport_id();

        return Passenger(passenger_name, passport_id);
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

    vector<CompactAssignment> get_compact_assignments()
    {
        vector<CompactAssignment> assignments;
        while (true)
        {
            auto input = read_line();
            if (stringutil::trim(input) == "0") { break; }

            try
            {
                assignments.push_back(parsers::parse_compact_assignment(input));
            }
            catch(const InvalidInputError &e)
            {
                std::cerr << "    Error: " << e.what() << endl;;
            } 
        }

        return assignments;
    }

    CompactAssignment::CompactAssignment(const string &passenger_name, const string &passport_id, const SeatLocation &seat_location)
        : passenger(passenger_name, passport_id), location { seat_location } {};

    bool CompactAssignment::equals(const CompactAssignment &other) const
    {
        return ((passenger == other.passenger) && (location == other.location));
    }

    string CompactAssignment::to_string() const
    {
        return (passenger.get_name() + "/" + passenger.get_passport_id() + "/" + location.to_string());
    }

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
            auto seat_location = parse_seat_location(input_segments.at(2));

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
