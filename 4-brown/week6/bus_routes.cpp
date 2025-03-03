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
            os << indent2 << R"("request_id": )" << *resp.request_id << ",\n";

            if (!resp.info.coordinate)
            {
                os << indent2 << R"("error_message": "not found")" << '\n';
                os << indent1 << (&response == &responses.back() ? "}\n" : "},\n");
                continue;
            }

            os << indent2 << R"("buses": [)" << '\n';
            for (const auto& bus : resp.info.buses)
            {
                os << indent3 << R"(")" << bus << (&bus == &*resp.info.buses.rbegin() ? R"(")" : R"(",)") << '\n';
            }
            os << indent2 << "]\n";
        }
        else if (std::holds_alternative<GetRouteResponse>(response))
        {
            const auto& resp = std::get<GetRouteResponse>(response);
            os << indent2 << R"("request_id": )" << *resp.request_id << ",\n";

            if (resp.route.stops.empty())
            {
                os << indent2 << R"("error_message": "not found")" << '\n';
                os << indent1 << (&response == &responses.back() ? "}\n" : "},\n");
                continue;
            }

            const auto& r = resp.route;
            os << indent2 << R"("route_length": )" << *r.length << ",\n";
            os << indent2 << R"("curvature": )" << std::setprecision(6) << *r.curvature << ",\n";
            os << indent2 << R"("stop_count": )" << r.stops.size() << ",\n";
            os << indent2 << R"("unique_stop_count": )" << r.unique_stops_count << '\n';
        }

        os << indent1 << (&response == &responses.back() ? "}\n" : "},\n");
    }

    os << "]\n";
}

int main()
{
    using namespace Transport;

    const Json::Document doc = Json::Load(std::cin);
    const auto& requests = doc.GetRoot().AsMap();
    Database db;

    for (const auto& node : requests.at("base_requests").AsArray())
    {
        WriteRequest::from_json(node)->execute(db);
    }

    std::vector<Response> responses;

    for (const auto& node : requests.at("stat_requests").AsArray())
    {
        responses.push_back(ReadRequest::from_json(node)->execute(db));
    }

    print_responses(responses, std::cout);
    return 0;
}
