#include <fstream>
#include <iomanip>
#include <iostream>

int main()
{
    std::ifstream ifs("input.txt");

    if (!ifs.is_open())
    {
        return 1;
    }

    double d;
    std::cout << std::fixed << std::setprecision(3);

    while (ifs >> d)
    {
        std::cout << d << std::endl;
    }

    ifs.close();

    return 0;
}
