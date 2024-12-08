#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
    if (std::distance(range_begin, range_end) < 2)
    {
        return;
    }

    std::vector<typename RandomIt::value_type> cur_range(range_begin, range_end);

    const auto range_mid = cur_range.begin() + static_cast<int>(cur_range.size()) / 2;
    MergeSort(cur_range.begin(), range_mid);
    MergeSort(range_mid, cur_range.end());

    std::merge(cur_range.begin(), range_mid, range_mid, cur_range.end(), range_begin);
}

int main()
{
    std::vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
    MergeSort(std::begin(v), std::end(v));
    for (int x : v)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    return 0;
}
