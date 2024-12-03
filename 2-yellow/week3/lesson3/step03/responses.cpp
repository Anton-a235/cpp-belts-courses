#include "responses.h"

#include <ostream>

using namespace std;

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
