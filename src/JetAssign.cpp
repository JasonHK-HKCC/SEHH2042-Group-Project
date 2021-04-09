#ifdef __unix__
    #include <termios.h>
#endif

#include <execinfo.h>

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
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
using std::flush;
using std::istream;

// optional
using std::optional;

// string
using std::string;

#define JET_ROW_LENGTH 13

#define JET_COLUMN_LENGTH 6

#define STRINGIFY(expression) #expression

#define STRINGIFY_VALUE(value) STRINGIFY(value)

void handler()
{
    void *trace_elems[20];
    int trace_elem_count(backtrace( trace_elems, 20 ));
    char **stack_syms(backtrace_symbols( trace_elems, trace_elem_count ));
    for ( int i = 0 ; i < trace_elem_count ; ++i )
    {
        std::cerr << stack_syms[i] << "\n";
    }
    free( stack_syms );

    exit(1);
}

namespace numericutil
{
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    bool is_even(const T &value);
}

/**
 * Utility functions for std::string.
 **/
namespace stringutil
{
    using std::vector;

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
                string name() const { return m_name; }

                /**
                 * Returns the passport ID of the passenger.
                 **/
                string passport_id() const { return m_passport_id; }

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
                string m_name;

                /**
                 * The passport ID of the passenger.
                 **/
                string m_passport_id;
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
                size_t row() const { return m_row; }

                /**
                 * Returns the column location of the seat.
                 **/
                size_t column() const { return m_column; }

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
                bool operator==(const SeatLocation &other) const { return equals(other); }

                /**
                 * Determine whether two instances represent different seat location.
                 *
                 * @param other The other instance.
                 **/
                bool operator!=(const SeatLocation &other) const { return !equals(other); }

                string to_string() const;

                operator string() const { return to_string(); }

                friend std::ostream& operator<<(std::ostream& os, const SeatLocation& location);

            private:
                /**
                 * The row location of the seat.
                 **/
                size_t m_row;

                /**
                 * The column location of the seat.
                 **/
                size_t m_column;
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

                void remove(const Passenger &passenger);

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

    namespace output
    {
        namespace messages
        {
            using core::Passenger;
            using core::SeatLocation;

            string confirm_reassignment_for_assigned_passenger(const Passenger& passenger, const SeatLocation& old_location, const SeatLocation& new_location);
        }
    }

    /**
     * The input component.
     **/
    namespace input
    {
        using std::vector;

        using core::Passenger;
        using core::SeatLocation;

        class AssignmentRequest
        {
            public:
                AssignmentRequest(const Passenger &passenger, const SeatLocation &location);

                AssignmentRequest(const string &passenger_name, const string &passport_id, const SeatLocation &seat_location);

                Passenger passenger() const { return m_passenger; }

                SeatLocation location() const { return m_location; }

                bool is_same_passenger(const AssignmentRequest &other) const;

                bool equals(const AssignmentRequest &other) const;

                bool operator ==(const AssignmentRequest &other) const { return equals(other); }

                bool operator !=(const AssignmentRequest &other) const { return !equals(other); }

                string to_string() const;

            private:
                Passenger m_passenger;
                SeatLocation m_location;
        };

        void wait_for_enter(const string &message = "Press ENTER to continue...");

        /**
         * Read the user's input until an EOL character was received.
         **/
        string read_line();

        bool get_confirmation(const string& message);

        bool get_confirmation(const string& message, bool default_value);

        long get_menu_option(long max);

        long get_menu_option(long min, long max);

        string get_passenger_name();

        string get_passport_id();

        Passenger get_passenger();

        SeatLocation get_seat_location();

        vector<AssignmentRequest> get_compact_assignments();

        /**
         * The input parsers component.
         **/
        namespace parsers
        {
            bool parse_confirmation(const string &input);

            bool parse_confirmation(const string &input, bool default_value);

            long parse_menu_option(const string &input);

            string parse_passenger_name(const string &input);

            string parse_passport_id(const string &input);

            SeatLocation parse_seat_location(const string &input);

            AssignmentRequest parse_compact_assignment(const string &input);
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
    using std::map;
    using std::set;
    using std::vector;

    using jetassign::seating_plan;
    using jetassign::core::SeatLocation;

    using jetassign::input::AssignmentRequest;
    using jetassign::input::get_confirmation;
    using jetassign::input::get_compact_assignments;

    using jetassign::output::messages::confirm_reassignment_for_assigned_passenger;

    auto requests = get_compact_assignments();
    cout << '\n';

    const auto print_nothing_committed = []()
    {
        cout << "No requests could be committed.\n";
        jetassign::input::wait_for_enter("Press ENTER to return to the main menu...");
    };

    if (requests.size() == 0)
    {
        return print_nothing_committed();
    }

    typedef vector<AssignmentRequest> RequestsVector;

    map<SeatLocation, bool, std::not_equal_to<SeatLocation>> occupation_states;

    RequestsVector successful_requests;

    RequestsVector unsuccessful_requests_assigned;
    RequestsVector unsuccessful_requests_occupied;

    auto is_occupied = [&occupation_states](const SeatLocation &location)
    {
        return occupation_states.count(location)
            ? occupation_states[location]
            : seating_plan.is_occupied(location);
    };

    for (auto request : requests)
    {
        auto passenger = request.passenger();
        auto location = request.location();

        if (seating_plan.is_assigned(request.passenger()))
        {
            auto assigned_location = *(seating_plan.location_of(passenger));
            occupation_states[assigned_location] = true;

            if (!get_confirmation(confirm_reassignment_for_assigned_passenger(passenger, assigned_location, location), false))
            {
            unsuccessful_requests_assigned.push_back(request);
        }
        else if (is_occupied(location))
        {
                occupation_states[location] = true;
                unsuccessful_requests_occupied.push_back(request);
            }
            else
            {
                occupation_states[location] = true;
                occupation_states[assigned_location] = false;

                successful_requests.push_back(request);
            }
        }
        else if (is_occupied(location))
        {
            occupation_states[location] = true;
            unsuccessful_requests_occupied.push_back(request);
        }
        else
        {
            occupation_states[location] = true;
            successful_requests.push_back(request);
        }
    }

    const auto successful_count = successful_requests.size();
    const auto unsuccessful_assigned_count = unsuccessful_requests_assigned.size();
    const auto unsuccessful_occupied_count = unsuccessful_requests_occupied.size();

    const auto print_requests_list = [](const RequestsVector& requests, size_t depth = 0)
    {
        for (auto request : requests)
        {
            cout << string(2 * depth, ' ') << "- " << request.to_string() << "\n";
        }
    };

    if (successful_count > 0)
    {
        cout << "These requests will be committed:\n";
        print_requests_list(successful_requests);

        cout << "\n";
    }

    if ((unsuccessful_assigned_count > 0) || (unsuccessful_occupied_count > 0))
    {
        cout << "These requests will be dropped:\n";

        if (unsuccessful_assigned_count > 0)
        {
            cout << "- Already assigned:\n";
            print_requests_list(unsuccessful_requests_assigned, 1);
        }

        if (unsuccessful_occupied_count > 0)
        {
            cout << "- Seat was occupied:\n";
            print_requests_list(unsuccessful_requests_occupied, 1);
        }

        cout << "\n";
    }

    if (successful_count == 0)
    {
        return print_nothing_committed();
    }

    if (get_confirmation("Are you sure to commit the requests?", false))
    {
        for (auto request : successful_requests)
        {
            if (seating_plan.is_assigned(request.passenger()))
            {
                seating_plan.remove(request.passenger());
            }

            seating_plan.assign(request.location(), request.passenger());
        }

        cout << "Done, " << successful_count << " " << ((successful_count == 1) ? "request was" : "requests were") << " committed.\n";
    }
    else
    {
        cout << "Cancelled, no requests were committed.\n";
    }

    jetassign::input::wait_for_enter("Press ENTER to return to the main menu...");
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

    SeatingPlan::SeatingPlan() {}

    bool SeatingPlan::is_occupied(const SeatLocation &location) const
    {
        return ((bool) seating_plan.at(location.row()).at(location.column()));
    }

    bool SeatingPlan::is_assigned(const Passenger &passenger) const
    {
        return ((bool) this->location_of(passenger));
    }

    const optional<Passenger> SeatingPlan::at(const SeatLocation &location) const
    {
        return seating_plan.at(location.row()).at(location.column());
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

        seating_plan.at(location.row()).at(location.column()) = passenger;
    }

    void SeatingPlan::remove(const SeatLocation &location)
    {
        if (this->is_occupied(location))
        {
            seating_plan.at(location.row()).at(location.column()) = std::nullopt;
        }
    }

    void SeatingPlan::remove(const Passenger &passenger)
    {
        if (this->is_assigned(passenger))
        {
            const auto location = *(this->location_of(passenger));
            seating_plan.at(location.row()).at(location.column()) = std::nullopt;
        }
    }

    Passenger::Passenger(const string &name, const string &passport_id)
        : m_name { name }, m_passport_id { passport_id } {}

    bool Passenger::equals(const Passenger &other) const
    {
        return ((m_name == other.m_name) && (m_passport_id == other.m_passport_id));
    }

    #define RANGE_ERROR_MESSAGE(name, max) "The range of the " #name " must between 0 (inclusive) and " STRINGIFY(max) " (exclusive)."

    #define ROW_RANGE_ERROR_MESSAGE RANGE_ERROR_MESSAGE(row, JET_ROW_LENGTH)

    #define COLUMN_RANGE_ERROR_MESSAGE RANGE_ERROR_MESSAGE(column, JET_COLUMN_LENGTH)

    string SeatLocation::row_to_string(size_t row)
    {
        if (row >= JET_ROW_LENGTH)
        {
            throw range_error(ROW_RANGE_ERROR_MESSAGE);
        }

        return std::to_string(row + 1);
    }

    string SeatLocation::column_to_string(size_t column)
    {
        if (column >= JET_COLUMN_LENGTH)
        {
            throw range_error(COLUMN_RANGE_ERROR_MESSAGE);
        }

        return string(1, (char) ('A' + column));
    }

    SeatLocation::SeatLocation(size_t row, size_t column)
        : m_row { row }, m_column { column }
    {
        if (row >= JET_ROW_LENGTH)
        {
            throw range_error(ROW_RANGE_ERROR_MESSAGE);
        }
        else if (column >= JET_COLUMN_LENGTH)
        {
            throw range_error(COLUMN_RANGE_ERROR_MESSAGE);
        }
    }

    bool SeatLocation::equals(const SeatLocation &other) const
    {
        return ((m_row == other.m_row) && (m_column == other.m_column));
    }

    string SeatLocation::to_string() const
    {
        return (row_to_string(m_row) + column_to_string(m_column));
    }

    std::ostream& operator<<(std::ostream& os, const SeatLocation& location)
    {
        os << location.to_string();
        return os;
    }

    #undef COLUMN_RANGE_ERROR_MESSAGE
    #undef ROW_RANGE_ERROR_MESSAGE
    #undef RANGE_ERROR_MESSAGE
}

namespace jetassign::exceptions
{

}

namespace jetassign::output
{
    namespace messages
    {
        string confirm_reassignment_for_assigned_passenger(const Passenger& passenger, const SeatLocation& old_location, const SeatLocation& new_location)
        {
            string message;
            return message
                .append(passenger.name())
                .append(" (")
                .append(passenger.passport_id())
                .append(") was already assigned to ")
                .append(old_location)
                .append(", would you like to move the passenger to ")
                .append(new_location)
                .append(" if the seat was available?");
        }
    }
}

namespace jetassign::input
{
    using exceptions::InvalidInputError;

    void wait_for_enter(const string &message)
    {
        cout << message << flush;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    string read_line()
    {
        string line;
        std::getline(cin, line);

        return line;
    }

    bool get_confirmation(const string& message)
    {
        while (true)
        {
            cout << message << " [y/n] " << flush;
            auto input = read_line();

            try
            {
                return parsers::parse_confirmation(input);
            }
            catch(const InvalidInputError& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }

    bool get_confirmation(const string& message, bool default_value)
    {
        while (true)
        {
            cout << message << " [" << (default_value ? "Y/n" : "y/N") << "] " << flush;
            auto input = read_line();

            try
            {
                return parsers::parse_confirmation(input, default_value);
            }
            catch(const InvalidInputError& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
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

    vector<AssignmentRequest> get_compact_assignments()
    {
        vector<AssignmentRequest> requests;
        while (true)
        {
            cout << "> ";
            auto input = read_line();
            if (stringutil::trim(input) == "0") { break; }

            try
            {
                auto request = parsers::parse_compact_assignment(input);
                requests.erase(
                    std::remove_if(
                        requests.begin(),
                        requests.end(),
                        [&](const AssignmentRequest &element) { return element.is_same_passenger(request); }),
                    requests.end());

                requests.push_back(parsers::parse_compact_assignment(input));
            }
            catch(const InvalidInputError &e)
            {
                std::cerr << "    Error: " << e.what() << endl;
            }
        }

        return requests;
    }

    AssignmentRequest::AssignmentRequest(const Passenger &passenger, const SeatLocation &location)
        : m_passenger { passenger }, m_location { location } {}

    AssignmentRequest::AssignmentRequest(const string &passenger_name, const string &passport_id, const SeatLocation &seat_location)
        : AssignmentRequest(Passenger(passenger_name, passport_id), seat_location) {};

    bool AssignmentRequest::is_same_passenger(const AssignmentRequest &other) const
    {
        return (m_passenger == other.m_passenger);
    }

    bool AssignmentRequest::equals(const AssignmentRequest &other) const
    {
        return ((m_passenger == other.m_passenger) && (m_location == other.m_location));
    }

    string AssignmentRequest::to_string() const
    {
        return (m_passenger.name() + "/" + m_passenger.passport_id() + "/" + m_location.to_string());
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

        bool parse_confirmation(const string &input)
        {
            const auto confirmation = stringutil::trim(input);
            if (confirmation.empty())
            {
                throw EmptyInputError("Please enter a command.");
            }

            switch (confirmation.at(0))
            {
                case 'Y':
                case 'y':
                    return true;

                case 'N':
                case 'n':
                    return false;

                default:
                    throw MalformedInputError("Invalid response. Please enter a correct command.");
            }
        }

        bool parse_confirmation(const string &input, bool default_value)
        {
            try
            {
                return parse_confirmation(input);
            }
            catch (const EmptyInputError& e)
            {
                return default_value;
            }
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

        AssignmentRequest parse_compact_assignment(const string &input)
        {
            const auto input_segments = stringutil::split(stringutil::trim(input), kCompactAssignmentSeparator);
            if (input_segments.size() != 3)
            {
                throw exceptions::MalformedInputError(R"(The assignment entry should be formatted as "<Name>/<Passport ID>/<Seat Location>".)");
            }

            auto passenger_name = parse_passenger_name(input_segments.at(0));
            auto passport_id = parse_passport_id(input_segments.at(1));
            auto seat_location = parse_seat_location(input_segments.at(2));

            return AssignmentRequest(passenger_name, passport_id, seat_location);
        }
    }
}

namespace numericutil
{
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    bool is_even(const T &value)
    {
        return ((value % 2) == 0);
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
