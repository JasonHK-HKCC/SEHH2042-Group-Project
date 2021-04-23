/**
 * Copyright (c) 2021 Jason Kwok, Ben Ho, Ben Yip, Harry Lam, and Hins To.
 *
 * Licensed under the GNU Affero General Public License, Version 3.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of the License at
 *
 *     https://github.com/JasonHK-HKCC/SEHH2042-Group-Project/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 **/

//       _     _________  _________  ________  ____  _____  _________  _____   ___   ____  _____  _
//      / \   |  _   _  ||  _   _  ||_   __  ||_   \|_   _||  _   _  ||_   _|.'   `.|_   \|_   _|| |
//     / _ \  |_/ | | \_||_/ | | \_|  | |_ \_|  |   \ | |  |_/ | | \_|  | | /  .-.  \ |   \ | |  | |
//    / ___ \     | |        | |      |  _| _   | |\ \| |      | |      | | | |   | | | |\ \| |  | |
//  _/ /   \ \_  _| |_      _| |_    _| |__/ | _| |_\   |_    _| |_    _| |_\  `-'  /_| |_\   |_ |_|
// |____| |____||_____|    |_____|  |________||_____|\____|  |_____|  |_____|`.___.'|_____|\____|(_)
//
// - C++17 IS REQUIRED to compile this source code!
// - C++17 IS REQUIRED to compile this source code!
// - C++17 IS REQUIRED to compile this source code!
//
// This source code has been checked using continuous integration (CI) pipeline, compiled with MSVC,
// in a Windows VM. Thus, this source code SHOULD and WILL compile, if not, please confirm the
// compiler you are using was up-to-date.
//

#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <random>
#include <regex>
#include <set>
#include <string>
#include <thread>
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

#define SECTION_SEPARATOR "\n\n"

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
        enum class TicketClass
        {
            kFirst,
            kBusiness,
            kEconomy,
        };

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

                TicketClass ticket_class() const;

                /**
                 * Determine whether two instances represent the same seat location.
                 *
                 * @param other The other instance.
                 **/
                bool equals(const SeatLocation& other) const;

                bool less_than(const SeatLocation &other) const;

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

                bool operator <(const SeatLocation &other) const { return less_than(other); }

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
                typedef optional<Passenger> value_type;

                typedef value_type& reference;

                typedef const value_type& const_reference;

                SeatingPlan();

                /**
                 * Determine whether the seat was already occupied by a passenger.
                 *
                 * @param location The location of the seat.
                 **/
                bool is_occupied(const SeatLocation &location) const noexcept;

                bool is_occupied(size_t row, size_t column) const noexcept;

                bool is_assigned(const string &passport_id) const noexcept;

                /**
                 * Determine whether a seat was already assigned to the passenger.
                 **/
                bool is_assigned(const Passenger &passenger) const noexcept;

                /**
                 * Returns the passenger who was assigned to the given seat.
                 *
                 * @param location The location of the seat.
                 **/
                const const_reference at(const SeatLocation &location) const;

                const const_reference at(size_t row, size_t column) const;

                optional<SeatLocation> location_of(const string &passport_id) const;

                optional<SeatLocation> location_of(const Passenger &passenger) const;

                /**
                 * Assign a passenger to a specific seat.
                 *
                 * @param location  The location of the seat.
                 * @param passenger The passenger to be assigned.
                 **/
                void assign(const SeatLocation &location, const_reference passenger);

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

        string to_string(TicketClass ticket_class) noexcept;
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

        class PassengerAssignedError : public runtime_error
        {

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

    namespace output
    {
        template<size_t TSize>
        using MenuOptions = array<string, TSize>;

        /**
         * @brief A container for storing menu and its fixed number of options.
         *
         * @tparam TMenuSize Number of options.
         **/
        template<size_t TMenuSize>
        struct Menu
        {
            /**
             * The title of the menu.
             **/
            string                 title;
            MenuOptions<TMenuSize> options;
        };

        class Spinner
        {
            public:
                static constexpr auto default_spinner = R"(|/-\)";

                Spinner();

                Spinner(string spinner);

                char spinner() const;

                char spin(bool frozen = false);

                friend std::ostream& operator<<(std::ostream& os, Spinner& spinner);

            private:
                string m_spinner;

                size_t m_index;
        };

        template<size_t TMenuSize>
        void print_menu(const Menu<TMenuSize>& menu);

        string build_progress_bar(size_t progress, size_t size);

        namespace messages
        {
            using std::vector;

            using core::Passenger;
            using core::SeatLocation;

            using input::AssignmentRequest;

            string confirm_reassignment_for_assigned_passenger(const Passenger& passenger, const SeatLocation& old_location, const SeatLocation& new_location);

            string report_committed_requests(size_t count);
        }
    }

    /**
     * The seating plan of the airplane.
     **/
    auto seating_plan = core::SeatingPlan();
}

long main_menu();

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
long show_details();

void show_details_passenger();

void show_details_class();

void save_and_exit();

#ifndef _TEST
int main(int argc, const char* argv[])
{
    static const auto welcome_message =
        R"(     _    _            _                    ____             _   _              )" "\n"
        R"(    / \  (_)_ __ _ __ | | __ _ _ __   ___  / ___|  ___  __ _| |_(_)_ __   __ _  )" "\n"
        R"(   / _ \ | | '__| '_ \| |/ _` | '_ \ / _ \ \___ \ / _ \/ _` | __| | '_ \ / _` | )" "\n"
        R"(  / ___ \| | |  | |_) | | (_| | | | |  __/  ___) |  __/ (_| | |_| | | | | (_| | )" "\n"
        R"( /_/   \_\_|_|  | .__/|_|\__,_|_| |_|\___| |____/ \___|\__,_|\__|_|_| |_|\__, | )" "\n"
        R"(                |_|                                                      |___/  )" "\n"
        R"(     _            _                                  _                          )" "\n"
        R"(    / \   ___ ___(_) __ _ _ __  _ __ ___   ___ _ __ | |_                        )" "\n"
        R"(   / _ \ / __/ __| |/ _` | '_ \| '_ ` _ \ / _ \ '_ \| __|                       )" "\n"
        R"(  / ___ \\__ \__ \ | (_| | | | | | | | | |  __/ | | | |_                        )" "\n"
        R"( /_/   \_\___/___/_|\__, |_| |_|_| |_| |_|\___|_| |_|\__|    VERSION 1.0.0      )" "\n"
        R"(                    |___/                                                       )" "\n";

    cout << welcome_message;

    long selection;
    do
    {
        switch ((selection = main_menu()))
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
                do
                {
                    switch ((details_selection = show_details()))
                    {
                        case 1:
                            show_details_passenger();
                            break;
                        case 2:
                            show_details_class();
                            break;
                        case 3:
                            break;
                    }
                }
                while (details_selection != 3);

                break;
            }
            case 6:
                save_and_exit();
                break;
        }
    }
    while (selection != 6);

    return 0;
}
#endif

long main_menu()
{
    using jetassign::input::get_menu_option;
    using jetassign::output::Menu;
    using jetassign::output::print_menu;

    cout << SECTION_SEPARATOR;

    static const Menu<6> menu =
    {
        "Main Menu",
        {{
            "Add an assignment",
            "Delete an assignment",
            "Add assignments in batch",
            "Show latest seating plan",
            "Show details",
            "Exit",
        }},
    };

    print_menu(menu);
    return get_menu_option(menu.options.size());
}

void add_an_assignment()
{
    using jetassign::seating_plan;

    using jetassign::input::wait_for_enter;
    using jetassign::input::get_confirmation;
    using jetassign::input::get_passenger;
    using jetassign::input::get_seat_location;

    do
    {
        auto passenger = get_passenger();

        auto location = get_seat_location();
        while (seating_plan.is_occupied(location))
        {
            cout << "The seat was already taken by another passenger.\n ";

            if (!get_confirmation("Would you want to assign the passenger to another seat?", true))
            {
                break;
            }

            location = get_seat_location();
        }

        if (seating_plan.is_occupied(location))
        {
            cout << "Canceled, the seating plan was not updated.\n\n";
            continue;
        }

        //assign the seat
        seating_plan.assign(location, passenger);
        cout << "Done, the seating plan was updated.\n\n";
    }
    while (get_confirmation("Do you want to assign another passenger?", true));
}

void delete_an_assignment()
{
    using jetassign::seating_plan;

    using jetassign::input::wait_for_enter;
    using jetassign::input::get_confirmation;
    using jetassign::input::get_passenger;

    do
    {
        auto passenger = get_passenger();
        if (seating_plan.is_assigned(passenger))
        {
            auto location = seating_plan.location_of(passenger);
            if (get_confirmation("Are you sure to remove the passenger from the seating plan?", false))
            {
                seating_plan.remove(*location);

                cout << "Done, the passenger was removed from the seating plan.\n";
            }
        }
        else
        {
            cout << "No such passenger exist!\n";
        }
    }
    while (get_confirmation("Do you want to remove another passenger?", true));
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

    namespace messages = jetassign::output::messages;

    do
    {
        cout << SECTION_SEPARATOR
             << "Assign multiple passengers to the seating plan at once.\n"
             << '\n';

        auto requests = get_compact_assignments();

        // Jump to the end of the loop early if no requests were received.
        if (requests.size() == 0)
        {
            cout << '\n'
                 << "No requests could be committed.\n";

            continue;
        }

        typedef vector<AssignmentRequest> RequestsVector;

        map<SeatLocation, bool> occupation_states;

        RequestsVector valid_requests;

        RequestsVector invalid_requests_assigned;
        RequestsVector invalid_requests_occupied;

        const auto is_occupied = [&](const SeatLocation& location)
        {
            return occupation_states.count(location)
                ? occupation_states[location]
                : seating_plan.is_occupied(location);
        };

        auto newline_before_reassignment_confirmation = true;
        for (auto request : requests)
        {
            auto passenger = request.passenger();
            auto location = request.location();

            if (seating_plan.is_assigned(request.passenger()))
            {
                auto assigned_location = *(seating_plan.location_of(passenger));
                occupation_states[assigned_location] = true;

                if (newline_before_reassignment_confirmation)
                {
                    cout << '\n';
                    newline_before_reassignment_confirmation = false;
                }

                if (!get_confirmation(messages::confirm_reassignment_for_assigned_passenger(passenger, assigned_location, location), true))
                {
                    invalid_requests_assigned.push_back(request);
                }
                else if (is_occupied(location))
                {
                    occupation_states[location] = true;
                    invalid_requests_occupied.push_back(request);
                }
                else
                {
                    occupation_states[assigned_location] = false;
                    occupation_states[location] = true;

                    valid_requests.push_back(request);
                }
            }
            else if (is_occupied(location))
            {
                occupation_states[location] = true;
                invalid_requests_occupied.push_back(request);
            }
            else
            {
                occupation_states[location] = true;
                valid_requests.push_back(request);
            }
        }

        /** The number of valid requests. */
        const auto valid_count = valid_requests.size();
        /** The number of invalid requests, which is because the passenger was assigned a seat. */
        const auto invalid_assigned_count = invalid_requests_assigned.size();
        /** The number of invalid requests, which is because the seat was occupied. */
        const auto invalid_occupied_count = invalid_requests_occupied.size();

        const auto print_requests_list = [](const RequestsVector& requests, size_t depth = 0)
        {
            for (auto request : requests)
            {
                cout << string(2 * depth, ' ') << "- " << request.to_string() << '\n';
            }
        };

        // List the valid requests, if any.
        if (valid_count > 0)
        {
            cout << '\n'
                 << "These requests will be committed:\n";

            print_requests_list(valid_requests);
        }

        // List the invalid requests, if any.
        if ((invalid_assigned_count > 0) || (invalid_occupied_count > 0))
        {
            cout << '\n'
                 << "These requests will be dropped:\n";

            if (invalid_assigned_count > 0)
            {
                cout << "- Already assigned:\n";
                print_requests_list(invalid_requests_assigned, 1);
            }

            if (invalid_occupied_count > 0)
            {
                cout << "- Seat was occupied:\n";
                print_requests_list(invalid_requests_occupied, 1);
            }
        }

        // Jump to the end of the loop early if no valid requests.
        if (valid_count == 0)
        {
            cout << '\n'
                 << "No requests could be committed.\n";

            continue;
        }

        if (get_confirmation("\nAre you sure to commit the requests?", true))
        {
            for (auto request : valid_requests)
            {
                if (seating_plan.is_assigned(request.passenger()))
                {
                    seating_plan.remove(request.passenger());
                }

                seating_plan.assign(request.location(), request.passenger());
            }

            cout << messages::report_committed_requests(valid_count) << '\n'
                 << '\n';
        }
        else
        {
            cout << "Cancelled, no requests were committed.\n"
                 << '\n';
        }
    }
    while (get_confirmation("Do you want to assign another batch of passengers?", true));
}

void show_latest_seating_plan()
{
    using std::left;
    using std::right;
    using std::setw;

    using jetassign::seating_plan;
    using jetassign::input::wait_for_enter;

    cout << SECTION_SEPARATOR
         << "This is the latest overall seat assignment for the plane:\n"
         << '\n';

    static const auto kColumnWidth = 3;
    static const auto kFirstColumnWidth = 3;

    static const auto kLegendSymbolWidth = 3;
    static const auto kLegendSymbolNameWidth = 13;

    static const auto kEmptySymbol = '*';
    static const auto kOccupiedSymbol = 'X';

    cout << right;

    // Prints the header row.
    cout << setw(kFirstColumnWidth) << ' ';
    for (auto column = 0; column < JET_COLUMN_LENGTH; column++)
    {
        cout << setw(kColumnWidth) << ((char) ('A' + column));
    }
    cout << '\n';

    // Prints each row of the seating plan.
    for (auto row = 0; row < JET_ROW_LENGTH; row++)
    {
        // Prints the row number.
        cout << setw(kFirstColumnWidth) << (row + 1);

        // Prints the occupation state of each column for the row.
        for (auto column = 0; column < JET_COLUMN_LENGTH; column++)
        {
            cout << setw(kColumnWidth) << (seating_plan.is_occupied(row, column) ? kOccupiedSymbol : kEmptySymbol);
        }

        cout << '\n';
    }
    cout << '\n';

    // Prints the legend for the seating plan.
    cout << left
         << "Legend:\n"
         << setw(kLegendSymbolWidth) << kEmptySymbol    << setw(kLegendSymbolNameWidth) << "Empty"
         << setw(kLegendSymbolWidth) << kOccupiedSymbol << setw(kLegendSymbolNameWidth) << "Occupied"
         << '\n'
         << '\n';

    wait_for_enter("Press ENTER to return to the main menu...");
}

long show_details()
{
    using jetassign::input::get_menu_option;
    using jetassign::output::Menu;
    using jetassign::output::print_menu;

    cout << SECTION_SEPARATOR;

    static const Menu<3> menu =
    {
        "Details",
        {{
            "Passenger",
            "Class",
            "Back",
        }},
    };

    print_menu(menu);
    return get_menu_option(menu.options.size());
}

void show_details_passenger()
{
    using jetassign::seating_plan;
    using jetassign::core::to_string;
    using jetassign::input::wait_for_enter;
    using jetassign::input::get_confirmation;
    using jetassign::input::get_passport_id;

    do
    {
        cout << SECTION_SEPARATOR
             << "Search for a particular passenger using his/her passport ID.\n";
        const auto passport_id = get_passport_id();
        cout << '\n';

        if (seating_plan.is_assigned(passport_id))
        {
            // Prints the ticket information of the passenger with a matching passport ID, if exists.

            const auto location = *(seating_plan.location_of(passport_id));

            cout << "A matching passenger was found!\n"
                 << "Passenger Name: " << seating_plan.at(location)->name() << '\n'
                 << "Passport    ID: " << passport_id << '\n'
                 << "Seat  Location: " << location << " (" << to_string(location.ticket_class()) << " Class)\n"
                 << '\n';

            wait_for_enter();
        }
        else
        {
            cout << "No matching passenger were found.\n";
        }
    }
    while (get_confirmation("Do you want to search for another passenger?", true));
}

void show_details_class()
{
    using std::left;
    using std::right;
    using std::setw;

    using jetassign::seating_plan;
    using jetassign::core::SeatLocation;
    using jetassign::input::wait_for_enter;
    using jetassign::input::get_confirmation;
    using jetassign::input::get_menu_option;
    using jetassign::output::Menu;
    using jetassign::output::print_menu;

    do
    {
        cout << SECTION_SEPARATOR
             << "List the passengers of a particular ticket class.\n"
             << '\n';

        size_t starting_row;
        size_t ending_row;

        static const Menu<4> menu =
        {
            "Ticket Class",
            {{
                "First Class",
                "Business Class",
                "Economy Class",
                "Back",
            }},
        };

        print_menu(menu);
        switch (get_menu_option(menu.options.size()))
        {
            case 1:
                starting_row = 0;
                ending_row   = 2;
                break;

            case 2:
                starting_row = 2;
                ending_row   = 7;
                break;

            case 3:
                starting_row = 7;
                ending_row   = 13;
                break;

            case 4:
                return;
        }

        cout << '\n'
             << "+------+---------------------------------------------------------------------+\n"
             << "| Seat | Passenger Name                                                      |\n"
             << "+------+---------------------------------------------------------------------+\n";

        static const auto kLocationColumnWidth = 4;
        static const auto kPassengerNameColumnWidth = 67;

        for (auto row = starting_row; row < ending_row; row++)
        {
            for (auto column = 0; column < JET_COLUMN_LENGTH; column++)
            {
                const auto location = SeatLocation(row, column);

                const auto passenger_name = seating_plan.is_occupied(location)
                    ? seating_plan.at(location)->name()
                    : "vacant";

                cout << "| "
                     << setw(kLocationColumnWidth) << right << location
                     << " | "
                     << setw(kPassengerNameColumnWidth) << left << passenger_name
                     << " |\n";
            }
        }

        cout << "+------+---------------------------------------------------------------------+\n"
             << '\n';

        wait_for_enter();
    }
    while (get_confirmation("Do you want to list the passengers of another ticket class?", true));
}

void save_and_exit()
{
    using std::flush;
    using std::left;
    using std::right;
    using std::setw;

    using std::random_device;
    using std::default_random_engine;
    using std::uniform_int_distribution;

    using namespace std::chrono_literals;

    using jetassign::input::wait_for_enter;
    using jetassign::output::Spinner;
    using jetassign::output::build_progress_bar;

    namespace this_thread = std::this_thread;

    cout << SECTION_SEPARATOR
         << "Upload the seating plan to the central database.\n"
         << '\n';

    const auto execute_operation = [](const string& operation, int max_step)
    {
        random_device device;
        default_random_engine engine(device());

        auto spinner = Spinner();

        const auto print_progress = [&](size_t progress)
        {
            if (progress > 100) { progress = 100; }
            const auto is_finished = (progress == 100);

            cout << '\r'
                 << (is_finished ? 'O' : spinner.spin()) << " "
                 << operation << ": "
                 << build_progress_bar(progress, 58) << " "
                 << setw(3) << right << progress << '%'
                 << (is_finished ? '\n' : '\0')
                 << flush;
        };

        size_t progress = 0;
        print_progress(progress);

        uniform_int_distribution progress_step(1, max_step);
        uniform_int_distribution progress_freeze(0, 3);

        do
        {
            this_thread::sleep_for(100ms);

            if (!progress_freeze(engine)) { progress += progress_step(engine); }
            print_progress(progress);
        }
        while (progress < 100);
    };

    execute_operation("Preparing", 40);
    execute_operation("Uploading", 15);

    cout << '\n'
         << "The seating plan was uploaded to the central database successfully!\n"
         << SECTION_SEPARATOR;

    wait_for_enter("Press ENTER to leave the application...");
}

namespace jetassign::core
{
    using std::range_error;

    SeatingPlan::SeatingPlan() {}

    bool SeatingPlan::is_occupied(const SeatLocation &location) const noexcept
    {
        return this->is_occupied(location.row(), location.column());
    }

    bool SeatingPlan::is_occupied(size_t row, size_t column) const noexcept
    {
        return ((bool) seating_plan.at(row).at(column));
    }

    bool SeatingPlan::is_assigned(const string &passport_id) const noexcept
    {
        return ((bool) this->location_of(passport_id));
    }

    bool SeatingPlan::is_assigned(const Passenger &passenger) const noexcept
    {
        return ((bool) this->location_of(passenger));
    }

    SeatingPlan::const_reference SeatingPlan::at(const SeatLocation &location) const
    {
        return this->at(location.row(), location.column());
    }

    SeatingPlan::const_reference SeatingPlan::at(size_t row, size_t column) const
    {
        return seating_plan.at(row).at(column);
    }

    optional<SeatLocation> SeatingPlan::location_of(const string &passport_id) const
    {
        for (auto row = 0; row < JET_ROW_LENGTH; row++)
        {
            for (auto column = 0; column < JET_COLUMN_LENGTH; column++)
            {
                const auto maybe_passenger = seating_plan.at(row).at(column);
                if (maybe_passenger && (maybe_passenger->passport_id() == passport_id))
                {
                    return SeatLocation(row, column);
                }
            }
        }

        return std::nullopt;
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

    void SeatingPlan::assign(const SeatLocation &location, const_reference passenger)
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

    TicketClass SeatLocation::ticket_class() const
    {
        if (m_row < 2)
        {
            return TicketClass::kFirst;
        }
        else if (m_row < 7)
        {
            return TicketClass::kBusiness;
        }
        else
        {
            return TicketClass::kEconomy;
        }
    }

    bool SeatLocation::equals(const SeatLocation& other) const
    {
        return ((m_row == other.m_row) && (m_column == other.m_column));
    }

    bool SeatLocation::less_than(const SeatLocation &other) const
    {
        return ((m_row < other.m_row) || ((m_row == other.m_row) && (m_column < other.m_column)));
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

    string to_string(TicketClass ticket_class) noexcept
    {
        switch (ticket_class)
        {
            case TicketClass::kFirst:
                return "First";
            case TicketClass::kBusiness:
                return "Business";
            case TicketClass::kEconomy:
                return "Economy";
        }
    }
}

namespace jetassign::exceptions
{

}

namespace jetassign::output
{
    using std::range_error;

    Spinner::Spinner() : Spinner(default_spinner) {}

    Spinner::Spinner(string spinner)
        : m_spinner { spinner }, m_index { 0 } {}

    char Spinner::spinner() const
    {
        return (m_spinner.size() == 0) ? '\0' : m_spinner[m_index % m_spinner.size()];
    }

    char Spinner::spin(bool frozen)
    {
        if (m_spinner.size() == 0) { return '\0'; }

        const auto index = (frozen ? m_index : m_index++) % m_spinner.size();
        return m_spinner[index];
    }

    std::ostream& operator<<(std::ostream& os, Spinner& spinner)
    {
        os << spinner.spin();
        return os;
    }

    template<size_t TMenuSize>
    void print_menu(const Menu<TMenuSize>& menu)
    {
        cout << "*** " << menu.title << " ***\n";

        const MenuOptions<TMenuSize> options = menu.options;

        for (auto i = 0; i < options.size(); i++)
        {
            const auto   ordinal = i + 1;
            const string option  = options.at(i);

            cout << "[" << ordinal << "] " << option << '\n';
        }

        cout << "*****************" << endl;
    }

    string build_progress_bar(size_t progress, size_t size)
    {
        if (size < 3)
        {
            throw range_error("The minimum size of the progress bar is 3.");
        }

        if (progress > 100) { progress = 100; }
        const auto is_finished = (progress == 100);

        const size_t progress_size = size - 2;
        const size_t progress_filled_size = progress_size * (progress / 100.0);
        const size_t progress_empty_size = progress_size - progress_filled_size - (is_finished ? 0 : 1);

        string progress_bar;
        return progress_bar
            .append("[")
            .append(string(progress_filled_size, '='))
            .append(string(!is_finished, '>'))
            .append(string(progress_empty_size, ' '))
            .append("]");
    }

    namespace messages
    {
        using std::to_string;

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

        string report_committed_requests(size_t count)
        {
            string message;
            return message
                .append("Done, ")
                .append(to_string(count))
                .append(" ")
                .append((count == 1) ? "request was" : "requests were")
                .append(" committed.");
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
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type*>
    bool is_even(const T &value)
    {
        return ((value % 2) == 0);
    }
}

namespace stringutil
{
    namespace
    {
        /**
         * The set of characters that considered whitespace characters.
         **/
        const auto kWhitespace = "\t\n\v\f\r\x20\xA0";
    }

    string trim(const string &input)
    {
        return trim_end(trim_start(input));
    }

    string trim_start(const string &input)
    {
        /** The position of the first character that not a whitespace character. */
        const auto index = input.find_first_not_of(kWhitespace);

        // Returns the substring of the input if there were some non-whitespace characters,
        // otherwise returns an empty string.
        return ((index == string::npos) ? "" : input.substr(index));
    }

    string trim_end(const string &input)
    {
        /** The position of the last character that not a whitespace character. */
        const auto index = input.find_last_not_of(kWhitespace);

        // Returns the substring of the input if there were some non-whitespace characters,
        // otherwise returns an empty string.
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
        // This function resembles JavaScript's String.prototype.split function.

        /** The segments that were split from the input. */
        vector<string> segments;

        if (separator.empty())
        {
            // Pushes the whole input as a segment if the separator was empty.
            segments.push_back(input);
        }
        else
        {
            /** The starting position for each substr and find calls. */
            size_t start_pos = 0;

            // Splits the input until it reaches its end.
            do
            {
                /** One of the segment that was split from the input. */
                auto segment = input.substr(start_pos, input.find(separator, start_pos) - start_pos);

                // Pushes the segment to the list and updates the starting position.
                segments.push_back(segment);
                start_pos += segment.length() + separator.length();
            }
            while (start_pos <= input.length());
        }

        return segments;
    }
}
