#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>

std::set<int>::const_iterator FindNearestElement(
    const std::set<int> &numbers,
    int border)
{
    const auto near2 = numbers.lower_bound(border);
    const auto near1 = (near2 == numbers.cbegin()) ? near2 : std::prev(near2);
    return (near2 != numbers.cend() && *near2 - border < border - *near1) ? near2 : near1;
}

int main()
{
    std::set<int> numbers = {1, 4, 6};
    std::cout << *FindNearestElement(numbers, 0) << " "
              << *FindNearestElement(numbers, 3) << " "
              << *FindNearestElement(numbers, 5) << " "
              << *FindNearestElement(numbers, 6) << " "
              << *FindNearestElement(numbers, 100) << std::endl;

    std::set<int> empty_set;

    std::cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << std::endl;
    return 0;
}
