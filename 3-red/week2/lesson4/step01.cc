#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class RouteManager
{
public:
    void AddRoute(int start, int finish)
    {
        reachable_lists_[start].insert(finish);
        reachable_lists_[finish].insert(start);
    }

    int FindNearestFinish(int start, int finish) const
    {
        int result = abs(start - finish);

        if (reachable_lists_.count(start) == 0 || reachable_lists_.at(start).empty())
        {
            return result;
        }

        const set<int> &reachable_stations = reachable_lists_.at(start);
        const auto station_after_finish = reachable_stations.lower_bound(finish);

        if (station_after_finish != reachable_stations.cend())
        {
            result = min(result, abs(*station_after_finish - finish));
        }

        if (station_after_finish != reachable_stations.cbegin())
        {
            result = min(result, abs(*prev(station_after_finish) - finish));
        }

        return result;
    }

private:
    map<int, set<int>> reachable_lists_;
};

int main()
{
    RouteManager routes;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id)
    {
        string query_type;
        cin >> query_type;

        int start, finish;
        cin >> start >> finish;

        if (query_type == "ADD")
        {
            routes.AddRoute(start, finish);
        }
        else if (query_type == "GO")
        {
            cout << routes.FindNearestFinish(start, finish) << "\n";
        }
    }

    return 0;
}
