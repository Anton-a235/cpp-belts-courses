#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

class ReadingManager
{
    static constexpr size_t MAX_PAGES_COUNT = 1000;

public:
    ReadingManager()
        : pages_to_users(MAX_PAGES_COUNT + 1)
    {
    }

    void Read(size_t user_id, size_t page_count)
    {
        if (users_to_pages.count(user_id) > 0)
        {
            --pages_to_users[users_to_pages[user_id]];
        }

        users_to_pages[user_id] = page_count;
        ++pages_to_users[page_count];
    }

    double Cheer(size_t user_id) const
    {
        if (users_to_pages.count(user_id) == 0)
        {
            return 0.0;
        }
        else if (users_to_pages.size() == 1)
        {
            return 1.0;
        }

        const auto users_count = accumulate(&pages_to_users.at(0),
                                            &pages_to_users.at(users_to_pages.at(user_id)),
                                            0ull);

        return static_cast<double>(users_count) / static_cast<double>(users_to_pages.size() - 1);
    }

private:
    unordered_map<size_t, size_t> users_to_pages;
    vector<size_t> pages_to_users;
};

int main()
{
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    size_t query_count;
    cin >> query_count;

    for (size_t query_id = 0; query_id < query_count; ++query_id)
    {
        string query_type;
        cin >> query_type;
        size_t user_id;
        cin >> user_id;

        if (query_type == "READ")
        {
            size_t page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        }
        else if (query_type == "CHEER")
        {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}
