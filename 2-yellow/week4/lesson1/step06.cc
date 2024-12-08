#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

template <typename T>
std::vector<T> FindGreaterElements(const std::set<T> &elements, const T &border)
{
    return std::vector<T>(elements.upper_bound(border), elements.cend());
}

int main()
{
    for (int x : FindGreaterElements(std::set<int>{1, 5, 7, 8}, 5))
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::string to_find = "Python";
    std::cout << FindGreaterElements(std::set<std::string>{"C", "C++"}, to_find).size() << std::endl;
    return 0;
}
