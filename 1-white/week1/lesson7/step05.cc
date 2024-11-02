#include <iostream>

int main()
{
    int a;
    int b;

    std::cin >> a >> b;

    if (b == 0)
    {
        std::cout << "Impossible\n";
    }
    else
    {
        std::cout << a / b << std::endl;
    }

    return 0;
}
