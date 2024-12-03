#include "query.h"

#include <istream>
#include <map>

using namespace std;

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
