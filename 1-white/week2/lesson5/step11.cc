#include <iostream>

#include <algorithm>
#include <map>
#include <vector>

int main()
{
    std::map<unsigned, std::vector<std::string>, std::greater<unsigned>> ways;
    unsigned n_reqs;

    std::cin >> n_reqs;

    while (n_reqs--)
    {
        std::vector<std::string> stops;
        unsigned n_stops;

        std::cin >> n_stops;

        stops.resize(n_stops);

        for (auto &stop : stops)
        {
            std::cin >> stop;
        }

        const auto it = std::find_if(ways.cbegin(), ways.cend(),
                                     [&stops](const auto &v)
                                     {
                                         return v.second == stops;
                                     });

        if (it != ways.cend())
        {
            std::cout << "Already exists for " << it->first << std::endl;
            continue;
        }

        unsigned new_bus = ways.empty() ? 1 : ways.begin()->first + 1;
        ways.insert(std::pair{new_bus, std::move(stops)});

        std::cout << "New bus " << new_bus << std::endl;
    }

    return 0;
}
