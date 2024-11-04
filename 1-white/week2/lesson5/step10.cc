#include <iostream>

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

int main()
{
    using cref_string_vector = std::vector<std::reference_wrapper<const std::string>>;
    std::map<std::string, cref_string_vector> ways;
    std::map<std::string, cref_string_vector> stops;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++)
    {
        unsigned stop_count;
        std::string stop;
        std::string bus;

        std::string req;
        std::cin >> req;

        if (req.compare("NEW_BUS") == 0)
        {
            std::cin >> bus >> stop_count;

            const auto [way_it, way_inserted] =
                ways.insert(std::pair{std::move(bus), cref_string_vector{}});

            while (stop_count--)
            {
                std::cin >> stop;

                const auto [stop_it, stop_inserted] =
                    stops.insert(std::pair{std::move(stop), cref_string_vector{}});

                way_it->second.push_back(std::cref(stop_it->first));
                stop_it->second.push_back(std::cref(way_it->first));
            }
        }
        else if (req.compare("BUSES_FOR_STOP") == 0)
        {
            std::cin >> stop;

            const auto it = stops.find(stop);

            if (it == stops.cend())
            {
                std::cout << "No stop\n";
                continue;
            }

            for (const auto &bus_ : it->second)
            {
                std::cout << bus_.get() << " ";
            }

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

            for (const auto &stop_ : way_it->second)
            {
                std::cout << "Stop " << stop_.get() << ": ";

                const auto stop_it = stops.find(stop_);

                if (stop_it->second.size() == 1)
                {
                    std::cout << "no interchange\n";
                    continue;
                }

                for (const auto &bus_ : stop_it->second)
                {
                    if (bus_.get() != bus)
                    {
                        std::cout << bus_.get() << " ";
                    }
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

                for (const auto &stop_ : way_)
                {
                    std::cout << stop_.get() << " ";
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
