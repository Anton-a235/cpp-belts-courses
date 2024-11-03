#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <type_traits>
#include <vector>

int main()
{
    const std::vector<unsigned> days_in_months =
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned cur_month = 0;

    std::map<unsigned, std::vector<std::string>> planner;
    std::string op;
    unsigned n;

    std::cin >> n;

    for (unsigned i = 0; i < n; i++)
    {
        std::string todo;
        unsigned day;

        std::cin >> op;

        if (op.compare("ADD") == 0)
        {
            std::cin >> day >> todo;
            planner[day].push_back(std::move(todo));
        }
        else if (op.compare("DUMP") == 0)
        {
            std::cin >> day;
            std::cout << planner[day].size();

            for (const auto &todo_ : planner[day])
            {
                std::cout << " " << todo_;
            }

            std::cout << std::endl;
        }
        else if (op.compare("NEXT") == 0)
        {
            unsigned next_month = (cur_month + 1) % 12;
            auto &todos_last_day = planner[days_in_months[next_month]];

            for (day = days_in_months[next_month] + 1;
                 day <= days_in_months[cur_month]; day++)
            {
                auto &todos = planner[day];
                std::move(todos.begin(), todos.end(), std::back_inserter(todos_last_day));
                todos.clear();
            }

            cur_month = next_month;
        }
        else
        {
            // Unknown operation
        }
    }

    return 0;
}
