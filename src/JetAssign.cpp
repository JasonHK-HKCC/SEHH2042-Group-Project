#include <algorithm>
#include <limits>
#include <string>
#include <array>
#include <vector>
#include <regex>
#include <iostream>

#include <climits>

using std::size_t;

// array
using std::array;

// exception
using std::exception;
using std::runtime_error;

// iostream
using std::cin;
using std::cout;
using std::endl;
using std::istream;

// optional
using std::optional;

// regex
using std::regex;
using std::regex_match;
using std::regex_search;

// string
using std::string;

// vector
using std::vector;

#define STRINGIFY(expression) #expression

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
        const auto kRowLength = 13;
        const auto kColumnLength = 6;
        const auto kJetSize = kRowLength * kColumnLength;

        const regex kPassportIdPattern("([0-9A-Z]+)", regex::icase);

        const regex kSeatLocationPattern("(1[0-3]|[1-9])([A-F])");

        const auto kCompactAssignmentSeparator = "/";
    }

    class SeatLocation
    {
        public:
            static const size_t unknown = ULONG_MAX;

            SeatLocation() {}

            SeatLocation(size_t index) : SeatLocation(index / kColumnLength, index % kColumnLength) {}

            SeatLocation(size_t row, size_t column) : row { row }, column { column }
            {
                if (row >= kRowLength)
                {
                    throw std::range_error("The range of the row must between 0 and 12 (inclusive).");
                }
                else if (column >= kColumnLength)
                {
                    throw std::range_error("The range of the column must between 0 and 5 (inclusive).");
                }
            };

            bool is_unknown() const
            {
                return ((row == unknown) || (column == unknown));
            }

            size_t get_row() const { return row; }

            size_t get_column() const { return column; }

            size_t get_index() const
            {
                return ((row * kColumnLength) + column);
            }

            bool equals(const SeatLocation &other) const
            {
                return ((row == other.row) && (column == other.column));
            }

            bool operator ==(const SeatLocation &other) const
            {
                return equals(other);
            }

            bool operator !=(const SeatLocation &other) const
            {
                return !equals(other);
            }

            string to_string() const
            {
                return this->is_unknown() ? "" : (std::to_string(row + 1) + ((char) ('A' + column)));
            }

        private:
            size_t row = unknown;
            size_t column = unknown;
    };

    struct Passenger
    {
        string name;
        string passport_id;
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
            bool is_occupied(const SeatLocation &location) const
            {
                return seating_plan.at(location.get_index()).has_value();
            }

            /**
             * Returns the passenger who was assigned to the given seat.
             * 
             * @param location The location of the seat.
             **/
            const optional<Passenger> at(const SeatLocation &location) const;

            SeatLocation location_of(const Passenger &passenger) const;

            /**
             * Assign a passenger to a specific seat.
             * 
             * @param location  The location of the seat.
             * @param passenger The passenger to be assigned.
             **/
            void assign(const SeatLocation &location, const Passenger &passenger);

        private:
            array<optional<Passenger>, kJetSize> seating_plan;
    };

    namespace exceptions
    {
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
                const SeatLocation get_location() const;

            private:
                SeatLocation location;
        };

        class InvalidInputError : public std::invalid_argument
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

    bool is_passport_id(string input)
    {
        return regex_search(input, kPassportIdPattern);
    }

    bool parse_passenger_name(string input, string &passenger_name)
    {
        const auto trimmed_input = stringutil::trim(input);
        if (trimmed_input.empty())
        {
            throw exceptions::EmptyInputError("The passenger's name must not be empty.");
        }

        passenger_name = trimmed_input;
        return true;
    }

    string parse_passenger_name_2(string input)
    {
        const auto passenger_name = stringutil::trim(input);
        if (passenger_name.empty())
        {
            throw exceptions::EmptyInputError("The passenger's name must not be empty.");
        }

        return passenger_name;
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

    string parse_passport_id_2(const string &input)
    {
        auto passport_id = stringutil::trim(input);
        if (passport_id.empty())
        {
            throw exceptions::EmptyInputError("The passport ID must not be empty.");
        }

        if (!regex_match(passport_id, kPassportIdPattern))
        {
            throw exceptions::MalformedInputError("Only alphanumeric characters were allowed.");
        }

        return passport_id;
    }

    bool parse_seat_location(const string &input, size_t &row, size_t &column)
    {
        auto trimmed_input = stringutil::to_uppercase(stringutil::trim(input));

        std::smatch match_result;
        if (!regex_match(input, match_result, kSeatLocationPattern))
        {
            throw exceptions::EmptyInputError("The passenger's name must not be empty.");
        }

        row = std::stoi(match_result.str(1)) - 1;
        column = match_result.str(2).at(0) - 'A';
        
        return true;
    }

    bool parse_seat_location(const string &input, SeatLocation &location)
    {
        size_t row, column;
        if (!parse_seat_location(input, row, column)) { return false; }

        location = SeatLocation(row, column);
        return true;
    }

    SeatLocation parse_seat_location_2(const string &input)
    {
        auto seat_location = stringutil::to_uppercase(stringutil::trim(input));
        if (seat_location.empty())
        {
            throw exceptions::EmptyInputError("The seat location must not be empty.");
        }

        std::smatch match_result;
        if (!regex_match(seat_location, match_result, kSeatLocationPattern))
        {
            throw exceptions::MalformedInputError("The seat location must be formatted as the row (1-13) followed by the column (A-F), e.g. 10D.");
        }

        auto row = std::stoi(match_result.str(1)) - 1;
        auto column = match_result.str(2).at(0) - 'A';
        
        return SeatLocation(row, column);
    }

    bool parse_compact_assignment(const string &input, string &passenger_name, string &passport_id, SeatLocation &seat_location)
    {
        const auto input_segments = stringutil::split(stringutil::trim(input), kCompactAssignmentSeparator);
        if (input_segments.size() != 3) { return false; }

        string parsed_passenger_name;
        if (!parse_passenger_name(input_segments.at(0), parsed_passenger_name))
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

        passenger_name = parsed_passenger_name;
        passport_id    = parsed_passport_id;
        seat_location  = parsed_seat_location;

        return true;
    }

    #pragma region SeatingPlan
    SeatingPlan::SeatingPlan()
    {

    }

    const optional<Passenger> SeatingPlan::at(const SeatLocation &location) const
    {
        return seating_plan.at(location.get_index());
    }

    // SeatLocation SeatingPlan::location_of(const Passenger &passenger) const
    // {
    //     // array<optional<Passenger>, kJetSize>::iterator element = std::find(seating_plan.begin(), seating_plan.end(), passenger);
    //     // if (element != seating_plan.end())
    //     // {
    //     //     return SeatLocation(std::distance(seating_plan.begin(), element));
    //     // }
    //     // else
    //     // {
    //     //     return SeatLocation();
    //     // }
    // }

    void SeatingPlan::assign(const SeatLocation &location, const Passenger &passenger)
    {
        if (this->is_occupied(location))
        {
            throw exceptions::SeatOccupiedError(location);
        }

        seating_plan.at(location.get_index()) = passenger;
    }
    #pragma endregion

    const SeatLocation exceptions::SeatOccupiedError::get_location() const
    {
        return location;
    }

    namespace ui
    {
        string get_line()
        {
            string line;
            std::getline(cin, line);

            return line;
        }

        string get_passenger_name()
        {
            while (true)
            {
                cout << "Passenger Name: ";
                auto input = get_line();

                try
                {
                    return parse_passenger_name_2(input);
                }
                catch(const exceptions::InvalidInputError &e)
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
                auto input = get_line();

                try
                {
                    return parse_passport_id_2(input);
                }
                catch(const exceptions::InvalidInputError &e)
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
                auto input = get_line();

                try
                {
                    return parse_seat_location_2(input);
                }
                catch(const exceptions::InvalidInputError &e)
                {
                    std::cerr << "    Error: " << e.what() << endl;;
                } 
            }
        }
    }
}

#ifndef _TEST
int main(int argc, const char* argv[])
{
    jetassign::ui::get_passenger_name();
    jetassign::ui::get_passport_id();
    jetassign::ui::get_seat_location();
    return 0;
}
#endif
