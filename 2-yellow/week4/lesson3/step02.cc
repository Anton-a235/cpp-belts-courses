#include <algorithm>
#include <ctime>
#include <iostream>
#include <map>
#include <numeric>

#pragma warning(disable : 4996)

std::istream &operator>>(std::istream &is, std::tm &tm)
{
    char ignore;
    tm = {};
    is >> tm.tm_year >> ignore >> tm.tm_mon >> ignore >> tm.tm_mday;
    tm.tm_year -= 1900;
    --tm.tm_mon;
    return is;
}

long long days_count(std::time_t &from, std::time_t &to)
{
    return (to - from) / 3600 / 24 + 1;
}

void add_day(std::time_t &time)
{
    time += 3600 * 24;
}

int main()
{
    std::map<std::time_t, double> budget;

    size_t n;
    std::cin >> n;
    std::cout.precision(25);

    while (n--)
    {
        std::string command;
        std::tm tm_from;
        std::tm tm_to;

        std::cin >> command >> tm_from >> tm_to;

        std::time_t from = std::mktime(&tm_from);
        std::time_t to = std::mktime(&tm_to);
        const auto days = days_count(from, to);

        if (command == "ComputeIncome")
        {
            const auto from_it = budget.lower_bound(from);
            const auto to_it = budget.upper_bound(to);
            std::cout << std::accumulate(from_it, to_it, 0.0,
                                         [](double sum, const auto &p)
                                         {
                                             return sum + p.second;
                                         })
                      << std::endl;
        }
        else if (command == "Earn")
        {
            double value;
            std::cin >> value;

            if (days <= 0)
            {
                break;
            }

            double earn_per_day = value / static_cast<double>(days);

            for (std::time_t cur_day = from; cur_day <= to; add_day(cur_day))
            {
                budget[cur_day] += earn_per_day;
            }
        }
        else
        {
            return 1;
        }
    }

    return 0;
}
