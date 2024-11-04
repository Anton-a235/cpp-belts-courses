#include <iostream>
#include <map>
#include <set>

int main()
{
    std::map<std::set<std::string>, size_t> buses;
    size_t n_reqs;

    std::cin >> n_reqs;

    while (n_reqs--)
    {
        std::set<std::string> stops;
        std::string stop;
        size_t n_stops;

        std::cin >> n_stops;

        while (n_stops--)
        {
            std::cin >> stop;
            stops.insert(std::move(stop));
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
