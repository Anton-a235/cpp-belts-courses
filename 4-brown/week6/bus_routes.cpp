#ifndef IGNORE_STD_HEADERS
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string_view>
#endif

#include "database.h"

void print_responses(const std::vector<Transport::Response>& responses, std::ostream& os)
{
    using namespace Transport;

    constexpr std::string_view indent1("  ");
    constexpr std::string_view indent2("    ");
    constexpr std::string_view indent3("      ");

    os << "[\n";

    for (const auto& response : responses)
    {
        os << indent1 << "{\n";

        if (std::holds_alternative<GetStopResponse>(response))
        {
            const auto& resp = std::get<GetStopResponse>(response);

            if (resp.request_id.has_value())
                os << indent2 << std::quoted("request_id") << ": " << *resp.request_id << ",\n";

            if (!resp.info.coordinate)
            {
                os << indent2 << std::quoted("error_message") << ": " << std::quoted("not found") << '\n';
                os << indent1 << (&response == &responses.back() ? "}\n" : "},\n");
                continue;
            }

            os << indent2 << std::quoted("buses") << ": [" << '\n';
            for (const auto& bus : resp.info.buses)
            {
                os << indent3 << std::quoted(bus) << (&bus == &*resp.info.buses.rbegin() ? "" : ",") << '\n';
            }
            os << indent2 << "]\n";
        }
        else if (std::holds_alternative<GetBusResponse>(response))
        {
            const auto& resp = std::get<GetBusResponse>(response);

            if (resp.request_id.has_value())
                os << indent2 << std::quoted("request_id") << ": " << *resp.request_id << ",\n";

            if (resp.route.stops.empty())
            {
                os << indent2 << std::quoted("error_message") << ": " << std::quoted("not found") << '\n';
                os << indent1 << (&response == &responses.back() ? "}\n" : "},\n");
                continue;
            }

            const auto& r = resp.route;
            os << indent2 << std::quoted("route_length") << ": " << *r.length << ",\n";
            os << indent2 << std::quoted("curvature") << ": " << std::setprecision(6) << *r.curvature << ",\n";
            os << indent2 << std::quoted("stop_count") << ": " << r.stops.size() << ",\n";
            os << indent2 << std::quoted("unique_stop_count") << ": " << r.unique_stops_count << '\n';
        }
        else if (std::holds_alternative<GetRouteResponse>(response))
        {
            const auto& resp = std::get<GetRouteResponse>(response);

            if (resp.request_id.has_value())
                os << indent2 << std::quoted("request_id") << ": " << *resp.request_id << ",\n";

            if (!resp.route.total_time.has_value())
            {
                os << indent2 << std::quoted("error_message") << ": " << std::quoted("not found") << '\n';
                os << indent1 << (&response == &responses.back() ? "}\n" : "},\n");
                continue;
            }

            os << indent2 << std::quoted("total_time") << ": ";
            os << std::setprecision(6) << *resp.route.total_time << ",\n";
            os << indent2 << std::quoted("items") << ": [" << '\n';

            for (const auto& item : resp.route.items)
            {
                constexpr std::string_view indent4("        ");

                os << indent3 << "{\n";
                if (std::holds_alternative<RouteWaitItem>(item))
                {
                    const auto& wait_item = std::get<RouteWaitItem>(item);
                    os << indent4 << std::quoted("type") << ": " << std::quoted("Wait") << ",\n";
                    os << indent4 << std::quoted("stop_name") << ": " << std::quoted(wait_item.stop_name) << ",\n";
                    os << indent4 << std::quoted("time") << ": " << std::setprecision(6) << wait_item.time << '\n';
                }
                else if (std::holds_alternative<RouteBusItem>(item))
                {
                    const auto& bus_item = std::get<RouteBusItem>(item);
                    os << indent4 << std::quoted("type") << ": " << std::quoted("Bus") << ",\n";
                    os << indent4 << std::quoted("bus") << ": " << std::quoted(bus_item.bus) << ",\n";
                    os << indent4 << std::quoted("span_count") << ": " << bus_item.span_count << ",\n";
                    os << indent4 << std::quoted("time") << ": " << std::setprecision(6) << bus_item.time << '\n';
                }
                os << indent3 << '}' << (&item == &*resp.route.items.rbegin() ? "" : ",") << '\n';
            }

            os << indent2 << "]\n";
        }

        os << indent1 << (&response == &responses.back() ? "}\n" : "},\n");
    }

    os << "]\n";
}

int main()
{
    using namespace Transport;

    const Json::Document doc = Json::Load(std::cin);
    const auto& root_node = doc.GetRoot().AsMap();
    Database db(root_node.at("routing_settings"));

    for (const auto& node : root_node.at("base_requests").AsArray())
    {
        WriteRequest::from_json(node)->execute(db);
    }

    std::vector<Response> responses;

    for (const auto& node : root_node.at("stat_requests").AsArray())
    {
        responses.push_back(ReadRequest::from_json(node)->execute(db));
    }

    print_responses(responses, std::cout);
    return 0;
}
