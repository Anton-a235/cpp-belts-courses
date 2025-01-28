#include "stats.h"

using namespace std;

Stats::Stats()
{
    method_stats_ = {
        {"GET", 0},
        {"PUT", 0},
        {"POST", 0},
        {"DELETE", 0},
        {"UNKNOWN", 0},
    };

    uri_stats_ = {
        {"/", 0},
        {"/order", 0},
        {"/product", 0},
        {"/basket", 0},
        {"/help", 0},
        {"unknown", 0},
    };
}

void Stats::AddMethod(string_view method)
{
    auto it = method_stats_.find(method);

    if (it != method_stats_.end())
    {
        ++it->second;
    }
    else
    {
        ++method_stats_["UNKNOWN"];
    }
}

void Stats::AddUri(string_view uri)
{
    auto it = uri_stats_.find(uri);

    if (it != uri_stats_.end())
    {
        ++it->second;
    }
    else
    {
        ++uri_stats_["unknown"];
    }
}

const map<string_view, int> &Stats::GetMethodStats() const
{
    return method_stats_;
}

const map<string_view, int> &Stats::GetUriStats() const
{
    return uri_stats_;
}

HttpRequest ParseRequest(string_view line)
{
    HttpRequest result{};

    auto from = line.find_first_not_of(' ', 0);
    line.remove_prefix(from);
    auto to = line.find_first_of(' ', 0);
    result.method = line.substr(0, to);
    line.remove_prefix(to);

    from = line.find_first_not_of(' ', 0);
    line.remove_prefix(from);
    to = line.find_first_of(' ', 0);
    result.uri = line.substr(0, to);
    line.remove_prefix(to);

    from = line.find_first_not_of(' ', 0);
    line.remove_prefix(from);
    to = line.find_first_of(' ', 0);
    result.protocol = line.substr(0, to);
    return result;
}
