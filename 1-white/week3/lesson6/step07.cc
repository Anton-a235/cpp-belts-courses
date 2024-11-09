// #include <iostream>

#include <algorithm>
#include <map>
#include <string>
#include <vector>

class Person
{
public:
    Person(const std::string &first_name, const std::string &last_name, int year)
    {
        first_names_[year] = first_name;
        last_names_[year] = last_name;
    }

    void ChangeFirstName(int year, const std::string &first_name)
    {
        if (year < std::prev(first_names_.cend())->first)
        {
            return;
        }

        first_names_[year] = first_name;
    }

    void ChangeLastName(int year, const std::string &last_name)
    {
        if (year < std::prev(last_names_.cend())->first)
        {
            return;
        }

        last_names_[year] = last_name;
    }

    std::string GetFullName(int year, bool with_history = false) const
    {
        const auto it_first = first_names_.lower_bound(year);
        const auto it_last = last_names_.lower_bound(year);

        const bool no_first = (it_first == first_names_.cend());
        const bool no_last = (it_last == last_names_.cend());

        if (no_first && no_last)
        {
            return "No person";
        }

        std::string first = no_first ? "with unknown first name" : it_first->second;
        std::string last = no_last ? "with unknown last name" : it_last->second;

        if (with_history)
        {
            const auto history_next = [](auto it_from, auto it_end, const auto &val) -> auto
            {
                return std::find_if(it_from, it_end,
                                    [&val](const auto &kv)
                                    {
                                        return kv.second != val;
                                    });
            };

            const auto append_history =
                [&history_next](std::string &name, auto it_cur, const auto &map) -> void
            {
                if (it_cur = history_next(it_cur, map.cend(), it_cur->second);
                    it_cur != map.cend())
                {
                    name += " (" + it_cur->second;

                    while ((it_cur = history_next(it_cur, map.cend(), it_cur->second)) !=
                           map.cend())
                    {
                        name += ", " + it_cur->second;
                    }

                    name += ")";
                }
            };

            append_history(first, it_first, first_names_);
            append_history(last, it_last, last_names_);
        }

        return no_first ? last + ' ' + first : first + ' ' + last;
    }

    std::string GetFullNameWithHistory(int year) const
    {
        return GetFullName(year, true);
    }

private:
    std::map<int, std::string, std::greater<int>> first_names_;
    std::map<int, std::string, std::greater<int>> last_names_;
};

// int main()
// {
//     Person person("Polina", "Sergeeva", 1960);
//     for (int year : {1959, 1960})
//     {
//         std::cout << person.GetFullNameWithHistory(year) << std::endl;
//     }

//     person.ChangeFirstName(1965, "Appolinaria");
//     person.ChangeLastName(1967, "Ivanova");
//     for (int year : {1965, 1967})
//     {
//         std::cout << person.GetFullNameWithHistory(year) << std::endl;
//     }

//     return 0;
// }
