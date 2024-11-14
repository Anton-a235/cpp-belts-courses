#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    size_t n;
    std::cin >> n;

    std::vector<int64_t> temper(n);

    for (auto &t : temper)
    {
        std::cin >> t;
    }

    int64_t avg = std::accumulate(temper.cbegin(), temper.cend(), 0ll) / static_cast<int64_t>(n);
    auto cnt = std::count_if(temper.cbegin(), temper.cend(),
                             [avg](const int64_t t)
                             {
                                 return t > avg;
                             });

    std::cout << cnt << std::endl;

    for (size_t i = 0; i < n; i++)
    {
        if (temper[i] > avg)
        {
            std::cout << i << " ";
        }
    }

    return 0;
}
