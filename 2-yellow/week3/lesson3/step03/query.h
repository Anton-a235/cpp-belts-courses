#pragma once

#include <iosfwd>
#include <string>
#include <vector>

enum class QueryType
{
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query
{
    QueryType type;
    std::string bus;
    std::string stop;
    std::vector<std::string> stops;
};

std::istream &operator>>(std::istream &is, Query &q);
