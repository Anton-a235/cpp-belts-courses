#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <string>

int main()
{
    int x;
    std::cin >> x;

    size_t n;
    (std::cin >> n).ignore();

    std::deque<std::string> result(1, std::to_string(x));

    while (n--)
    {
        std::string op;
        std::getline(std::cin, op);
        result.push_front("(");
        result.push_back(") ");
        result.push_back(op);
    }

    std::copy(result.cbegin(), result.cend(), std::ostream_iterator<std::string>(std::cout));

    return 0;
}
