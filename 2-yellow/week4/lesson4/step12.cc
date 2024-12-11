#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <string>

int main()
{
    int x;
    std::cin >> x;

    size_t n;
    (std::cin >> n).ignore();

    const std::map<char, int> op_priority{
        {'+', 0},
        {'-', 0},
        {'*', 1},
        {'/', 1},
    };

    std::deque<std::string> result(1, std::to_string(x));
    char prev_op = '*';

    while (n--)
    {
        std::string op;
        std::getline(std::cin, op);

        if (op_priority.at(op.front()) > op_priority.at(prev_op))
        {
            result.push_front("(");
            result.push_back(")");
        }

        result.push_back(' ' + op);
        prev_op = op.front();
    }

    std::copy(result.cbegin(), result.cend(), std::ostream_iterator<std::string>(std::cout));

    return 0;
}
