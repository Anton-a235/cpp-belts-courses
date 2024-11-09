#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v;
    unsigned n;

    std::cin >> n;
    v.resize(n);

    for (auto &i : v)
    {
        std::cin >> i;
    }

    std::sort(v.begin(), v.end(),
              [](const int i, const int j)
              {
                  return std::abs(i) < std::abs(j);
              });

    for (const auto &i : v)
    {
        std::cout << i << " ";
    }

    return 0;
}
