#include <cmath>
#include <iostream>

int main()
{
    double a;
    double b;
    double c;

    std::cin >> a >> b >> c;

    if (a == 0.0)
    {
        if (b != 0.0)
        {
            std::cout << -c / b << std::endl;
        }

        return 0;
    }

    double d = b * b - 4 * a * c;

    if (d == 0.0)
    {
        std::cout << -b / (2 * a) << std::endl;
    }
    else if (d > 0.0)
    {
        double sqrt_d = std::sqrt(d);
        std::cout << (-b - sqrt_d) / (2 * a) << " ";
        std::cout << (-b + sqrt_d) / (2 * a) << std::endl;
    }

    return 0;
}
