#ifndef IGNORE_STD_HEADERS
#include <iomanip>
#include <iostream>
#endif

#include "database.h"

void print_response(BusRoutes::Response resp, std::ostream& os)
{
    using namespace BusRoutes;

    if (std::holds_alternative<ConstRouteInfoPtr>(resp))
    {
        const auto& route_info = std::get<ConstRouteInfoPtr>(resp);

        if (route_info->stops.empty())
        {
            os << "Bus " << route_info->bus << ": not found\n";
            return;
        }

        os << std::setprecision(6) << "Bus " << route_info->bus << ": " << route_info->stops.size()
           << " stops on route, " << route_info->unique_stops_count << " unique stops, " << *route_info->length
           << " route length\n";
    }
}

int main()
{
    using namespace BusRoutes;

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
