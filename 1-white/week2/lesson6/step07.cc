#include <iostream>
#include <type_traits>
#include <unordered_set>

int main()
{
    std::unordered_set<std::string> set;
    unsigned n;

    std::cin >> n;

    while (n--)
    {
        std::string str;
        std::cin >> str;
        set.insert(std::move(str));
    }

    std::cout << set.size() << std::endl;

    return 0;
}
