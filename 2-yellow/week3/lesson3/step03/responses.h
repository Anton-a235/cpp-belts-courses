#pragma once

#include <iosfwd>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct BusesForStopResponse
{
    std::vector<std::string> buses;
};

std::ostream &operator<<(std::ostream &os, const BusesForStopResponse &r);

struct StopsForBusResponse
{
    std::vector<std::pair<std::string, BusesForStopResponse>> stops;
};

std::ostream &operator<<(std::ostream &os, const StopsForBusResponse &r);

struct AllBusesResponse
{
    std::map<std::string, std::vector<std::string>> buses;
};

std::ostream &operator<<(std::ostream &os, const AllBusesResponse &r);
