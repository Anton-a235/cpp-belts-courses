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

    const auto range_mid_1 = cur_range.begin() + static_cast<int>(cur_range.size()) / 3;
    const auto range_mid_2 = cur_range.begin() + static_cast<int>(cur_range.size()) * 2 / 3;

    MergeSort(cur_range.begin(), range_mid_1);
    MergeSort(range_mid_1, range_mid_2);
    MergeSort(range_mid_2, cur_range.end());

    std::vector<typename RandomIt::value_type> tmp_merged;
    std::merge(cur_range.begin(), range_mid_1, range_mid_1, range_mid_2, std::back_inserter(tmp_merged));
    std::merge(tmp_merged.begin(), tmp_merged.end(), range_mid_2, cur_range.end(), range_begin);
}

int main()
{
    std::vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
    MergeSort(std::begin(v), std::end(v));
    for (int x : v)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    return 0;
}
