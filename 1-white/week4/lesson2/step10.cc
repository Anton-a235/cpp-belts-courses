#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream ifs("input.txt");

    if (!ifs.is_open())
    {
        return 1;
    }

    std::string line;

    while (std::getline(ifs, line))
    {
        std::cout << line << std::endl;
    }

    ifs.close();

    return 0;
}
