#include <iostream>
#include <string>

int main()
{
    std::string str;
    std::cin >> str;

    auto pos = str.find('f');

    if (pos == std::string::npos)
    {
        std::cout << "-2\n";
        return 0;
    }

    pos = str.find('f', pos + 1);

    if (pos == std::string::npos)
    {
        std::cout << "-1\n";
        return 0;
    }

    std::cout << pos << std::endl;

    return 0;
}
