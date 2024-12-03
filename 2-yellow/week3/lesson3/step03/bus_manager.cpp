#include "bus_manager.h"

void BusManager::AddBus(const std::string &bus, const std::vector<std::string> &stops)
{
    buses_to_stops_[bus] = stops;

    for (const std::string &stop : stops)
    {
        stops_to_buses_[stop].push_back(bus);
    }
}

BusesForStopResponse BusManager::GetBusesForStop(const std::string &stop) const
{
    const auto it = stops_to_buses_.find(stop);

    if (it == stops_to_buses_.cend())
    {
        return {};
    }

    return {it->second};
}

StopsForBusResponse BusManager::GetStopsForBus(const std::string &bus) const
{
    const auto stops_it = buses_to_stops_.find(bus);
    StopsForBusResponse res;

    if (stops_it == buses_to_stops_.cend())
    {
        return res;
    }

    for (const std::string &stop : stops_it->second)
    {
        res.stops.emplace_back(stop, BusesForStopResponse{});

        for (const std::string &other_bus : stops_to_buses_.at(stop))
        {
            if (bus != other_bus)
            {
                res.stops.back().second.buses.push_back(other_bus);
            }
        }
    }

    return res;
}

AllBusesResponse BusManager::GetAllBuses() const
{
    return {buses_to_stops_};
}
