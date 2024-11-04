#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>

int main()
{
    std::map<std::string, std::set<std::string>> sinonims;
    unsigned n;

    std::cin >> n;

    while (n--)
    {
        std::string req;
        std::string word1;
        std::string word2;

        std::cin >> req;

        if (req.compare("ADD") == 0)
        {
            std::cin >> word1 >> word2;

            const auto [it1, inserted1] = sinonims.insert(std::pair{std::move(word1),
                                                                    std::set<std::string>{}});
            const auto [it2, inserted2] = sinonims.insert(std::pair{std::move(word2),
                                                                    std::set<std::string>{}});

            it1->second.insert(it2->first);
            it2->second.insert(it1->first);
        }
        else if (req.compare("COUNT") == 0)
        {
            std::cin >> word1;
            const auto it = sinonims.find(word1);
            std::cout << (it == sinonims.cend() ? 0 : it->second.size()) << std::endl;
        }
        else if (req.compare("CHECK") == 0)
        {
            std::cin >> word1 >> word2;
            const auto it = sinonims.find(word1);
            std::cout << (it != sinonims.cend() && it->second.count(word2) ? "YES" : "NO")
                      << std::endl;
        }
        else
        {
            // Unknown req
        }
    }

    return 0;
}
