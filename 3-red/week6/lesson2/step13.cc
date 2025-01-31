#include "test_runner.h"

#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
    const auto size = distance(range_begin, range_end);

    if (size < 2)
    {
        return;
    }

    vector<typename RandomIt::value_type> v(
        make_move_iterator(range_begin),
        make_move_iterator(range_end));

    auto mid1 = v.begin() + size / 3;
    auto mid2 = v.begin() + size * 2 / 3;

    MergeSort(v.begin(), mid1);
    MergeSort(mid1, mid2);
    MergeSort(mid2, v.end());

    vector<typename RandomIt::value_type> tmp;

    merge(make_move_iterator(v.begin()), make_move_iterator(mid1),
          make_move_iterator(mid1), make_move_iterator(mid2),
          back_inserter(tmp));

    merge(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()),
          make_move_iterator(mid2), make_move_iterator(v.end()),
          range_begin);
}

void TestIntVector()
{
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}
