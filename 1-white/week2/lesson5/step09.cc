#include <iostream>

#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

int main()
{
    std::map<std::string, std::string> capitals;
    int n;

    std::cin >> n;

    for (int i = 0; i < n; i++)
    {
        std::string req;
        std::string param1;
        std::string param2;

        std::cin >> req;

        if (req.compare("CHANGE_CAPITAL") == 0)
        {
            std::cin >> param1 >> param2; // country, new_capital

            const auto country_it = capitals.find(param1);
            const std::string old_capital = country_it != capitals.cend()
                                                ? std::move(country_it->second)
                                                : std::string{};

            const auto [it, inserted] = capitals.insert_or_assign(std::move(param1),
                                                                  std::move(param2));

            if (inserted)
            {
                std::cout << "Introduce new country " << it->first
                          << " with capital " << it->second << std::endl;
            }
            else
            {
                if (old_capital == it->second)
                {
                    std::cout << "Country " << it->first
                              << " hasn't changed its capital" << std::endl;
                }
                else
                {
                    std::cout << "Country " << it->first << " has changed its capital from "
                              << old_capital << " to " << it->second << std::endl;
                }
            }
        }
        else if (req.compare("RENAME") == 0)
        {
            std::cin >> param1 >> param2; // old_country_name, new_country_name

            if (param1 == param2 ||
                capitals.find(param1) == capitals.cend() ||
                capitals.find(param2) != capitals.cend())
            {
                std::cout << "Incorrect rename, skip\n";
                continue;
            }

            auto node = capitals.extract(param1);
            node.key() = std::move(param2);
            const auto [it, inserted, type] = capitals.insert(std::move(node));

            std::cout << "Country " << param1 << " with capital " << it->second
                      << " has been renamed to " << it->first << std::endl;
        }
        else if (req.compare("ABOUT") == 0)
        {
            std::cin >> param1; // country

            const auto country_it = capitals.find(param1);

            if (country_it == capitals.cend())
            {
                std::cout << "Country " << param1 << " doesn't exist\n";
                continue;
            }

            std::cout << "Country " << country_it->first
                      << " has capital " << country_it->second << std::endl;
        }
        else if (req.compare("DUMP") == 0)
        {
            if (capitals.size() == 0)
            {
                std::cout << "There are no countries in the world\n";
                continue;
            }

            for (const auto &[country, capital] : capitals)
            {
                std::cout << country << "/" << capital << " ";
            }

            std::cout << std::endl;
        }
        else
        {
            // Unknown request
        }
    }

    return 0;
}
