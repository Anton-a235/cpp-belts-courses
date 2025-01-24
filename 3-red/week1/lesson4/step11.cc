#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

#define UPDATE_FIELD(ticket, field, values) \
    {                                       \
        auto it = values.find(#field);      \
        if (it != values.end())             \
        {                                   \
            stringstream ss;                \
            ss << it->second;               \
            ss >> ticket.field;             \
        }                                   \
    }

inline bool operator<(const Date &lhs, const Date &rhs)
{
    return std::tie(lhs.year, lhs.month, lhs.day) <
           std::tie(rhs.year, rhs.month, rhs.day);
}

inline bool operator==(const Date &lhs, const Date &rhs)
{
    return std::tie(lhs.year, lhs.month, lhs.day) ==
           std::tie(rhs.year, rhs.month, rhs.day);
}

inline ostream &operator<<(ostream &os, const Date &d)
{
    return os << d.year << '-' << d.month << '-' << d.day;
}

std::istream &operator>>(std::istream &is, Date &date)
{
    int year;
    int month;
    int day;

    if ((is >> year) &&
        (is.peek() == '-' && is.ignore(1)) &&
        (is >> month) &&
        (is.peek() == '-' && is.ignore(1)) &&
        (is >> day))
    {
        date = {year, month, day};
    }

    return is;
}

inline bool operator<(const Time &lhs, const Time &rhs)
{
    return std::tie(lhs.hours, lhs.minutes) <
           std::tie(rhs.hours, rhs.minutes);
}

inline bool operator==(const Time &lhs, const Time &rhs)
{
    return std::tie(lhs.hours, lhs.minutes) ==
           std::tie(rhs.hours, rhs.minutes);
}

inline ostream &operator<<(ostream &os, const Time &t)
{
    return os << t.hours << ':' << t.minutes;
}

std::istream &operator>>(std::istream &is, Time &time)
{
    int hours;
    int minutes;

    if ((is >> hours) &&
        (is.peek() == ':' && is.ignore(1)) &&
        (is >> minutes))
    {
        time = {hours, minutes};
    }

    return is;
}

void TestUpdate()
{
    AirlineTicket t;
    t.price = 0;

    const map<string, string> updates1 = {
        {"departure_date", "2018-2-28"},
        {"departure_time", "17:40"},
    };
    UPDATE_FIELD(t, departure_date, updates1);
    UPDATE_FIELD(t, departure_time, updates1);
    UPDATE_FIELD(t, price, updates1);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 0);

    const map<string, string> updates2 = {
        {"price", "12550"},
        {"arrival_time", "20:33"},
    };
    UPDATE_FIELD(t, departure_date, updates2);
    UPDATE_FIELD(t, departure_time, updates2);
    UPDATE_FIELD(t, arrival_time, updates2);
    UPDATE_FIELD(t, price, updates2);

    // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
    // значения этих полей не должны измениться
    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 12550);
    ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestUpdate);
}
