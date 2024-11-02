// #include <iostream>

#include <algorithm>
#include <iterator>
#include <vector>

std::vector<int> Reversed(const std::vector<int> &v)
{
    std::vector<int> result;
    std::copy(v.crbegin(), v.crend(), std::back_inserter(result));
    return result;
}

// int main()
// {
//     std::vector<int> numbers = {1, 5, 3, 4, 2};
//     std::vector<int> result = Reversed(numbers);

//     // result должен оказаться равен {2, 4, 3, 5, 1}
//     for (const auto n : result)
//     {
//         std::cout << n << " ";
//     }

//     return 0;
// }
