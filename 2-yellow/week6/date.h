#pragma once

#include <iostream>
#include <sstream>

class Date
{
public:
    Date() noexcept;
    Date(int year, int month, int day);

    int GetYear() const noexcept;
    int GetMonth() const noexcept;
    int GetDay() const noexcept;

private:
    int year_;
    int month_;
    int day_;

    friend bool operator<(const Date &lhs, const Date &rhs);
    friend bool operator<=(const Date &lhs, const Date &rhs);
    friend bool operator>(const Date &lhs, const Date &rhs);
    friend bool operator>=(const Date &lhs, const Date &rhs);
    friend bool operator==(const Date &lhs, const Date &rhs);
    friend bool operator!=(const Date &lhs, const Date &rhs);
};

Date ParseDate(std::istream &is);

bool operator<(const Date &lhs, const Date &rhs);
bool operator<=(const Date &lhs, const Date &rhs);
bool operator>(const Date &lhs, const Date &rhs);
bool operator>=(const Date &lhs, const Date &rhs);
bool operator==(const Date &lhs, const Date &rhs);
bool operator!=(const Date &lhs, const Date &rhs);

std::istream &operator>>(std::istream &is, Date &date);
std::ostream &operator<<(std::ostream &os, const Date &date);
