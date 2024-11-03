#include <iostream>

#include <algorithm>
#include <numeric>
#include <vector>

int main()
{
    size_t n;
    std::cin >> n;

    std::vector<int> temper(n);

    for (auto &t : temper)
    {
        std::cin >> t;
    }

    int avg = std::accumulate(temper.cbegin(), temper.cend(), 0) / static_cast<int>(n);
    auto cnt = std::count_if(temper.cbegin(), temper.cend(),
                             [avg](const int t)
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
