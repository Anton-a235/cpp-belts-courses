#include <algorithm>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

int main()
{
    std::map<std::vector<std::string>, size_t> buses;
    size_t n_reqs;

    std::cin >> n_reqs;

    while (n_reqs--)
    {
        std::vector<std::string> stops;
        size_t n_stops;

        std::cin >> n_stops;
        stops.resize(n_stops);

        for (auto &stop : stops)
        {
            std::cin >> stop;
        }

        auto it = buses.find(stops);

        if (it != buses.cend())
        {
            std::cout << "Already exists for " << it->second << std::endl;
            continue;
        }

        std::tie(it, std::ignore) = buses.insert(std::pair{std::move(stops),
                                                           buses.size() + 1});

        std::cout << "New bus " << it->second << std::endl;
    }

    return 0;
}
