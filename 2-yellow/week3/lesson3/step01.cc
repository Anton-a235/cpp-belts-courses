#include "phone_number.h"

#include <exception>
#include <sstream>
#include <string>

using namespace std;

PhoneNumber::PhoneNumber(const string &international_number)
{
    istringstream ss(international_number);
    char c;

    if ((c = (char)ss.peek()) != '+' || !ss.ignore() ||
        !getline(ss, country_code_, '-') || country_code_.empty() ||
        !getline(ss, city_code_, '-') || city_code_.empty() ||
        !getline(ss, local_number_) || local_number_.empty())
    {
        throw invalid_argument("");
    }
}

string PhoneNumber::GetCountryCode() const
{
    return country_code_;
}

string PhoneNumber::GetCityCode() const
{
    return city_code_;
}

string PhoneNumber::GetLocalNumber() const
{
    return local_number_;
}

string PhoneNumber::GetInternationalNumber() const
{
    return '+' + country_code_ + '-' + city_code_ + '-' + local_number_;
}

// #include <iostream>

// int main()
// {
//     PhoneNumber pn("+7-495-111-22-33");
//     cout << pn.GetCountryCode() << endl;
//     cout << pn.GetCityCode() << endl;
//     cout << pn.GetLocalNumber() << endl;
//     cout << pn.GetInternationalNumber() << endl;
//     return 0;
// }
