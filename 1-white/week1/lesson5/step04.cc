#include <iostream>
#include <string>
#include <type_traits>

int main()
{
    std::string result;
    std::string in;

    std::cin >> result;

    while (std::cin >> in)
    {
        if (in < result)
        {
            result = std::move(in);
        }
    }

    std::cout << result << std::endl;

    return 0;
}
