#include "date.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

Date::Date() noexcept : Date(1, 1, 1) {}

Date::Date(int year, int month, int day)
    : year_(year), month_(month), day_(day)
{
    if (month < 1 || month > 12)
    {
        throw std::invalid_argument(
            std::string{"Month value is invalid: "}.append(std::to_string(month)));
    }

    if (day < 1 || day > 31)
    {
        throw std::invalid_argument(
            std::string{"Day value is invalid: "}.append(std::to_string(day)));
    }
}

int Date::GetYear() const noexcept
{
    return year_;
}

int Date::GetMonth() const noexcept
{
    return month_;
}

int Date::GetDay() const noexcept
{
    return day_;
}

Date ParseDate(std::istream &is)
{
    Date date;

    if (!(is >> date))
    {
        throw std::runtime_error("Wrong date format");
    }

    return date;
}

bool operator<(const Date &lhs, const Date &rhs)
{
    return std::tie(lhs.year_, lhs.month_, lhs.day_) <
           std::tie(rhs.year_, rhs.month_, rhs.day_);
}

bool operator<=(const Date &lhs, const Date &rhs)
{
    return std::tie(lhs.year_, lhs.month_, lhs.day_) <=
           std::tie(rhs.year_, rhs.month_, rhs.day_);
}

bool operator>(const Date &lhs, const Date &rhs)
{
    return std::tie(lhs.year_, lhs.month_, lhs.day_) >
           std::tie(rhs.year_, rhs.month_, rhs.day_);
}

bool operator>=(const Date &lhs, const Date &rhs)
{
    return std::tie(lhs.year_, lhs.month_, lhs.day_) >=
           std::tie(rhs.year_, rhs.month_, rhs.day_);
}

bool operator==(const Date &lhs, const Date &rhs)
{
    return std::tie(lhs.year_, lhs.month_, lhs.day_) ==
           std::tie(rhs.year_, rhs.month_, rhs.day_);
}

bool operator!=(const Date &lhs, const Date &rhs)
{
    return std::tie(lhs.year_, lhs.month_, lhs.day_) !=
           std::tie(rhs.year_, rhs.month_, rhs.day_);
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

std::ostream &operator<<(std::ostream &os, const Date &date)
{
    return os << std::setw(4) << std::setfill('0') << date.GetYear() << "-"
              << std::setw(2) << std::setfill('0') << date.GetMonth() << "-"
              << std::setw(2) << std::setfill('0') << date.GetDay();
}
