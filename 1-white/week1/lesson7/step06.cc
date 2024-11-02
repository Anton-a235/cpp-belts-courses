#include <iostream>

int main()
{
    double n, a, b, x, y;

    std::cin >> n >> a >> b >> x >> y;

    if (n > b)
    {
        std::cout << n * (1 - y / 100) << std::endl;
    }
    else if (n > a)
    {
        std::cout << n * (1 - x / 100) << std::endl;
    }
    else
    {
        std::cout << n << std::endl;
    }

    return 0;
}
