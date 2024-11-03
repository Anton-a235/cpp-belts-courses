#include <iostream>

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <vector>

int main()
{
    std::map<std::string, std::vector<std::reference_wrapper<const std::string>>> ways;
    std::vector<std::reference_wrapper<const std::string>> buses;
    std::unordered_set<std::string> stops;

    const auto buses_for_stop =
        [&ways, &buses](const std::string &stop, const std::string &bus_exclude = {}) -> unsigned
    {
        unsigned cnt = 0;

        for (const auto &b : buses)
        {
            const auto way = ways.find(b.get());

            if (std::find_if(way->second.cbegin(), way->second.cend(),
                             [&stop](const auto &v)
                             {
                                 return v.get() == stop;
                             }) != way->second.cend())
            {
                if (way->first == bus_exclude)
                {
                    continue;
                }

                std::cout << way->first << " ";
                ++cnt;
            }
        }

        return cnt;
    };

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++)
    {
        unsigned stop_count;
        std::string stop_k;
        std::string bus;

        std::string req;
        std::cin >> req;

        if (req.compare("NEW_BUS") == 0)
        {
            std::cin >> bus >> stop_count;

            const auto [way_it, way_inserted] =
                ways.insert(std::pair{std::move(bus),
                                      std::vector<std::reference_wrapper<const std::string>>{}});

            buses.push_back(std::cref(way_it->first));

            while (stop_count--)
            {
                std::cin >> stop_k;
                const auto [stop_it, stop_inserted] = stops.insert(std::move(stop_k));
                way_it->second.push_back(std::cref(*stop_it));
            }
        }
        else if (req.compare("BUSES_FOR_STOP") == 0)
        {
            std::cin >> stop_k;

            if (stops.find(stop_k) == stops.cend())
            {
                std::cout << "No stop\n";
                continue;
            }

            std::ignore = buses_for_stop(stop_k);
            std::cout << std::endl;
        }
        else if (req.compare("STOPS_FOR_BUS") == 0)
        {
            std::cin >> bus;

            const auto way_it = ways.find(bus);

            if (way_it == ways.cend())
            {
                std::cout << "No bus\n";
                continue;
            }

            for (const auto &stop : way_it->second)
            {
                std::cout << "Stop " << stop.get() << ": ";
                unsigned cnt = buses_for_stop(stop.get(), way_it->first);

                if (cnt == 0)
                {
                    std::cout << "no interchange";
                }

                std::cout << std::endl;
            }
        }
        else if (req.compare("ALL_BUSES") == 0)
        {
            if (ways.empty())
            {
                std::cout << "No buses\n";
                continue;
            }

            for (const auto &[bus_, way_] : ways)
            {
                std::cout << "Bus " << bus_ << ": ";

                for (const auto &stop : way_)
                {
                    std::cout << stop.get() << " ";
                }

                std::cout << std::endl;
            }
        }
        else
        {
            // Unknown request
        }
    }

    return 0;
}
