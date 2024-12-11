#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <tuple>
#include <type_traits>

int main()
{
    std::map<std::string, unsigned long long> budget{{{}, 0}};

    size_t n;
    std::cin >> n;

    while (n--)
    {
        std::string date;
        unsigned long long value;
        std::cin >> date >> value;

        budget[std::move(date)] += value;
    }

    unsigned long long sum = 0;
    std::for_each(std::next(budget.begin()), budget.end(),
                  [&sum](auto &p)
                  {
                      p.second += sum;
                      sum = p.second;
                  });

    std::cin >> n;

    while (n--)
    {
        std::string from;
        std::string to;
        std::cin >> from >> to;

        const auto from_it = std::prev(budget.lower_bound(from));
        const auto to_it = std::prev(budget.upper_bound(to));
        std::cout << to_it->second - from_it->second << std::endl;
    }

    return 0;
}
