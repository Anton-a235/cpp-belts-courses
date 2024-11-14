#include <cstdint>
#include <iostream>

int main()
{
    unsigned n;
    unsigned r;
    std::cin >> n >> r;

    uint64_t result = 0;

    while (n--)
    {
        uint64_t w, h, d;
        std::cin >> w >> h >> d;
        result += w * h * d * r;
    }

    std::cout << result << std::endl;

    return 0;
}
