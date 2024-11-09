// #include <iostream>

#include <algorithm>
#include <map>
#include <string>
#include <vector>

class Person
{
public:
    void ChangeFirstName(int year, const std::string &first_name)
    {
        first_names_[year] = first_name;
    }

    void ChangeLastName(int year, const std::string &last_name)
    {
        last_names_[year] = last_name;
    }

    std::string GetFullName(int year) const
    {
        const auto it_first = first_names_.lower_bound(year);
        const auto it_last = last_names_.lower_bound(year);

        const bool no_first = (it_first == first_names_.cend());
        const bool no_last = (it_last == last_names_.cend());

        if (no_first && no_last)
        {
            return "Incognito";
        }

        const std::string &first = no_first ? "with unknown first name"
                                            : it_first->second;

        const std::string &last = no_last ? "with unknown last name"
                                          : it_last->second;

        return no_first ? last + ' ' + first : first + ' ' + last;
    }

private:
    std::map<int, std::string, std::greater<int>> first_names_;
    std::map<int, std::string, std::greater<int>> last_names_;
};

// int main()
// {
//     Person person;

//     person.ChangeFirstName(1965, "Polina");
//     person.ChangeLastName(1967, "Sergeeva");

//     for (int year : {1900, 1965, 1990})
//     {
//         std::cout << person.GetFullName(year) << std::endl;
//     }

//     person.ChangeFirstName(1970, "Appolinaria");

//     for (int year : {1969, 1970})
//     {
//         std::cout << person.GetFullName(year) << std::endl;
//     }

//     person.ChangeLastName(1968, "Volkova");

//     for (int year : {1969, 1970})
//     {
//         std::cout << person.GetFullName(year) << std::endl;
//     }

//     return 0;
// }
