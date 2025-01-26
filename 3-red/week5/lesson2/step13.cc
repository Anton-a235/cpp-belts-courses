#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned n;
    std::cin >> n;

    std::list<unsigned> order;
    std::unordered_map<unsigned, std::list<unsigned>::iterator> num_iters;
    unsigned num, before;

    while (n--)
    {
        std::cin >> num >> before;
        const auto before_it = (num_iters.count(before) > 0) ? num_iters[before] : order.cend();
        num_iters[num] = order.insert(before_it, num);
    }

    for (const auto &num_ : order)
    {
        std::cout << num_ << ' ';
    }

    return 0;
}
