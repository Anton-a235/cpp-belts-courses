#include <iostream>

#include <algorithm>
#include <vector>

int main()
{
    std::vector<bool> queue;
    std::string op;
    size_t n;

    std::cin >> n;

    for (size_t i = 0; i < n; i++)
    {
        int param;

        std::cin >> op;

        if (op.compare("WORRY_COUNT") == 0)
        {
            std::cout << std::count(queue.cbegin(), queue.cend(), true)
                      << std::endl;

            continue;
        }

        std::cin >> param;

        if (op.compare("COME") == 0)
        {
            queue.resize(queue.size() + param);
        }
        else if (op.compare("WORRY") == 0)
        {
            queue[static_cast<size_t>(param)] = true;
        }
        else if (op.compare("QUIET") == 0)
        {
            queue[static_cast<size_t>(param)] = false;
        }
        else
        {
            // Unknown operation
        }
    }

    return 0;
}
