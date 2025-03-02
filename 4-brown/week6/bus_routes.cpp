#ifndef IGNORE_STD_HEADERS
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#endif

#include "database.h"

void print_response(Transport::Response response, std::ostream& os)
{
    using namespace Transport;

    if (std::holds_alternative<GetStopResponse>(response))
    {
        const auto& resp = std::get<GetStopResponse>(response);
        os << "Stop " << resp.stop << ": ";

        if (!resp.info.coordinate)
        {
            os << "not found\n";
            return;
        }

        if (resp.info.buses.empty())
        {
            os << "no buses\n";
            return;
        }

        os << "buses ";
        std::copy(resp.info.buses.cbegin(), resp.info.buses.cend(), std::ostream_iterator<std::string>(os, " "));
        os << '\n';
    }
    else if (std::holds_alternative<GetRouteResponse>(response))
    {
        const auto& resp = std::get<GetRouteResponse>(response);
        os << "Bus " << resp.bus << ": ";

        if (resp.route.stops.empty())
        {
            os << "not found\n";
            return;
        }

        const auto& r = resp.route;
        os << r.stops.size() << " stops on route, " << r.unique_stops_count << " unique stops, " << *r.length
           << " route length, " << std::setprecision(6) << *r.curvature << " curvature\n";
    }
}

int main()
{
    using namespace Transport;

    Database db;
    std::string line;
    int req_count;

    std::cin >> req_count;
    std::getline(std::cin, line);

    while (req_count--)
    {
        std::getline(std::cin, line);
        WriteRequest::from_string(line)->execute(db);
    }

    std::cin >> req_count;
    std::getline(std::cin, line);

    while (req_count--)
    {
        std::getline(std::cin, line);
        print_response(ReadRequest::from_string(line)->execute(db), std::cout);
    }

    return 0;
}
