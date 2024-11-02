// #include <iostream>

#include <algorithm>
#include <iterator>
#include <vector>

void Reverse(std::vector<int> &v)
{
    auto front = v.begin();
    auto back = v.end() - 1;
    auto mid = v.begin();

    std::advance(mid, static_cast<ptrdiff_t>(v.size() / 2));

    while (front != mid)
    {
        std::iter_swap(front++, back--);
    }
}

// int main()
// {
//     std::vector<int> numbers = {1, 5, 3, 4, 2};
//     Reverse(numbers);

//     // numbers должен оказаться равен {2, 4, 3, 5, 1}
//     for (const auto n : numbers)
//     {
//         std::cout << n << " ";
//     }

//     return 0;
// }
