#include "database.h"

#ifndef IGNORE_STD_HEADERS
#include <algorithm>
#include <charconv>
#include <iterator>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#endif

#include "geo.h"

namespace Transport
{

using std::string_view_literals::operator""sv;

void Database::add_stop(std::string stop, Geo::Coordinate coord, std::unordered_map<std::string, int> distances)
{
    road_distances_[stop].merge(std::move(distances));
    stops_[std::move(stop)].coordinate = coord;
}

const StopInfo& Database::get_stop(const std::string& stop) const
{
    static const StopInfo empty_info{};
    return stops_.count(stop) > 0 ? stops_.at(stop) : empty_info;
}

void Database::add_route(std::string bus, RouteInfo route)
{
    for (const auto& stop : route.stops)
        stops_[stop].buses.insert(bus);

    routes_[std::move(bus)] = std::move(route);
}

static double route_geo_length(const std::vector<std::string>& stops,
                               const std::unordered_map<std::string, StopInfo>& stops_info)
{
    double length = 0.0;

    for (auto it = stops.cbegin(); !stops.empty() && it != std::prev(stops.cend()); ++it)
        length += Geo::earth_distance(*stops_info.at(*it).coordinate, *stops_info.at(*std::next(it)).coordinate);

    return length;
}

static int route_road_length(
    const std::vector<std::string>& stops,
    const std::unordered_map<std::string, std::unordered_map<std::string, int>>& road_distances_)
{
    int length = 0;

    for (auto it = stops.cbegin(); !stops.empty() && it != std::prev(stops.cend()); ++it)
    {
        if (road_distances_.count(*it) > 0 && road_distances_.at(*it).count(*std::next(it)))
            length += road_distances_.at(*it).at(*std::next(it));
        else
            length += road_distances_.at(*std::next(it)).at(*it);
    }

    return length;
}

const RouteInfo& Database::get_route(const std::string& bus) const
{
    static const RouteInfo empty_info{};

    if (routes_.count(bus) == 0)
        return empty_info;

    auto& route_info = routes_.at(bus);

    if (!route_info.length.has_value())
        route_info.length = route_road_length(route_info.stops, road_distances_);

    if (!route_info.curvature.has_value())
        route_info.curvature = *route_info.length * 1.0 / route_geo_length(route_info.stops, stops_);

    return route_info;
}

static std::pair<std::string_view, char> get_word(std::string_view& str, std::string_view delims = " "sv)
{
    std::string_view word = str.substr(0, str.find_first_of(delims));
    char separator = str[word.size()];
    str.remove_prefix(std::min(word.size() + 1, str.size()));

    while (!str.empty() && str.front() == ' ')
        str.remove_prefix(1);

    while (!word.empty() && word.back() == ' ')
        word.remove_suffix(1);

    return {word, separator};
}

WriteRequestPtr WriteRequest::from_string(std::string_view str)
{
    std::string_view command = get_word(str).first;
    std::string_view name = get_word(str, ":"sv).first;

    if (command == "Stop"sv)
    {
        std::unordered_map<std::string, int> distances;
        std::string_view lat_str = get_word(str, ","sv).first;
        auto [lon_str, separator] = get_word(str, ","sv);
        double latitude = std::stod(std::string{lat_str.cbegin(), lat_str.cend()});
        double longitude = std::stod(std::string{lon_str.cbegin(), lon_str.cend()});

        while (separator == ',')
        {
            int dist;
            std::string_view stop_to;
            std::string_view dist_str = get_word(str, "m"sv).first;
            std::ignore = std::from_chars(dist_str.data(), dist_str.data() + dist_str.size(), dist);
            std::ignore = get_word(str); // to
            std::tie(stop_to, separator) = get_word(str, ","sv);
            distances[{stop_to.cbegin(), stop_to.cend()}] = dist;
        }

        return std::make_unique<AddStopRequest>(
            std::string{name.cbegin(), name.cend()}, Geo::Coordinate{latitude, longitude}, std::move(distances));
    }
    else if (command == "Bus"sv)
    {
        constexpr unsigned MAX_STOPS_COUNT_IN_ROUTE = 100;

        RouteInfo route{};
        route.stops.reserve(MAX_STOPS_COUNT_IN_ROUTE);

        std::unordered_set<std::string_view> unique_stops;
        char stops_separator = '\0';

        while (!str.empty())
        {
            const auto [stop, separator] = get_word(str, "->"sv);
            unique_stops.insert(stop);
            if (stops_separator == '\0')
                stops_separator = separator;
            route.stops.emplace_back(stop.cbegin(), stop.cend());
        }

        route.unique_stops_count = unique_stops.size();

        if (stops_separator == '-')
            std::copy(std::next(route.stops.crbegin()), route.stops.crend(), std::back_inserter(route.stops));

        return std::make_unique<AddRouteRequest>(std::string{name.cbegin(), name.cend()}, std::move(route));
    }

    return nullptr;
}

AddStopRequest::AddStopRequest(std::string stop, Geo::Coordinate coord, std::unordered_map<std::string, int> distances)
    : stop_(std::move(stop)), coord_(coord), distances_(std::move(distances))
{
}

void AddStopRequest::execute(Database& db) const
{
    db.add_stop(std::move(stop_), coord_, std::move(distances_));
}

AddRouteRequest::AddRouteRequest(std::string bus, RouteInfo route)
    : bus_(std::move(bus)), route_(std::move(route))
{
}

void AddRouteRequest::execute(Database& db) const
{
    db.add_route(std::move(bus_), std::move(route_));
}

ReadRequestPtr ReadRequest::from_string(std::string_view str)
{
    std::string_view command = get_word(str).first;
    std::string_view name = get_word(str, ":"sv).first;

    if (command == "Stop"sv)
        return std::make_unique<GetStopRequest>(std::string{name.cbegin(), name.cend()});
    else if (command == "Bus"sv)
        return std::make_unique<GetRouteRequest>(std::string{name.cbegin(), name.cend()});

    return nullptr;
}

Transport::GetStopRequest::GetStopRequest(std::string stop)
    : stop_(std::move(stop))
{
}

Response GetStopRequest::execute(const Database& db) const
{
    return GetStopResponse{std::move(stop_), db.get_stop(stop_)};
}

GetRouteRequest::GetRouteRequest(std::string bus)
    : bus_(std::move(bus))
{
}

Response GetRouteRequest::execute(const Database& db) const
{
    return GetRouteResponse{std::move(bus_), db.get_route(bus_)};
}

} // namespace Transport
