#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    size_t n;
    std::cin >> n;

    std::vector<int> v(n);
    std::iota(v.rbegin(), v.rend(), 1);

    do
    {
        for (const auto &a : v)
        {
            std::cout << a << " ";
        }

        std::cout << std::endl;
    } while (std::prev_permutation(v.begin(), v.end()));

    return 0;
}
