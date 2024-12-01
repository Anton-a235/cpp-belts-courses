#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <type_traits>

using namespace std;

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
    string bus;
    string stop;
    vector<string> stops;
};

istream &operator>>(istream &is, Query &q)
{
    static const map<string, QueryType> query_types{
        {"NEW_BUS", QueryType::NewBus},
        {"BUSES_FOR_STOP", QueryType::BusesForStop},
        {"STOPS_FOR_BUS", QueryType::StopsForBus},
        {"ALL_BUSES", QueryType::AllBuses}};

    Query req;
    string operation;
    int stop_count;

    if (is >> operation && query_types.find(operation) != query_types.cend())
    {
        req.type = query_types.at(operation);

        switch (req.type)
        {
        case QueryType::NewBus:
            if (!(is >> req.bus >> stop_count) || stop_count < 0)
            {
                return is;
            }
            req.stops.resize(static_cast<size_t>(stop_count));
            for (string &stop : req.stops)
            {
                if (!(is >> stop))
                {
                    return is;
                }
            }
            break;

        case QueryType::BusesForStop:
            if (!(is >> req.stop))
            {
                return is;
            }
            break;

        case QueryType::StopsForBus:
            if (!(is >> req.bus))
            {
                return is;
            }
            break;

        case QueryType::AllBuses:
            break;
        }

        q = move(req);
    }

    return is;
}

struct BusesForStopResponse
{
    vector<string> buses;
};

ostream &operator<<(ostream &os, const BusesForStopResponse &r)
{
    if (r.buses.empty())
    {
        os << "No stop";
        return os;
    }

    for (const auto &bus : r.buses)
    {
        os << bus << " ";
    }

    return os;
}

struct StopsForBusResponse
{
    vector<std::pair<string, BusesForStopResponse>> stops;
};

ostream &operator<<(ostream &os, const StopsForBusResponse &r)
{
    if (r.stops.empty())
    {
        os << "No bus";
        return os;
    }

    for (const auto &[stop, buses] : r.stops)
    {
        os << "Stop " << stop << ": ";

        if (buses.buses.empty())
        {
            os << "no interchange\n";
        }
        else
        {
            os << buses << endl;
        }
    }

    return os;
}

struct AllBusesResponse
{
    map<string, vector<string>> buses;
};

ostream &operator<<(ostream &os, const AllBusesResponse &r)
{
    if (r.buses.empty())
    {
        os << "No buses";
        return os;
    }

    for (const auto &[bus, stops] : r.buses)
    {
        os << "Bus " << bus << ": ";

        for (const auto &stop : stops)
        {
            os << stop << " ";
        }

        os << endl;
    }

    return os;
}

class BusManager
{
public:
    void AddBus(const string &bus, const vector<string> &stops)
    {
        buses_to_stops_[bus] = stops;

        for (const string &stop : stops)
        {
            stops_to_buses_[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string &stop) const
    {
        const auto it = stops_to_buses_.find(stop);

        if (it == stops_to_buses_.cend())
        {
            return {};
        }

        return {it->second};
    }

    StopsForBusResponse GetStopsForBus(const string &bus) const
    {
        const auto stops_it = buses_to_stops_.find(bus);
        StopsForBusResponse res;

        if (stops_it == buses_to_stops_.cend())
        {
            return res;
        }

        for (const string &stop : stops_it->second)
        {
            res.stops.emplace_back(stop, BusesForStopResponse{});

            for (const string &other_bus : stops_to_buses_.at(stop))
            {
                if (bus != other_bus)
                {
                    res.stops.back().second.buses.push_back(other_bus);
                }
            }
        }

        return res;
    }

    AllBusesResponse GetAllBuses() const
    {
        return {buses_to_stops_};
    }

public:
    map<string, vector<string>> buses_to_stops_;
    map<string, vector<string>> stops_to_buses_;
};

int main()
{
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i)
    {
        cin >> q;
        switch (q.type)
        {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }

    return 0;
}
