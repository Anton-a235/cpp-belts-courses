#include "database.h"

#ifndef IGNORE_STD_HEADERS
#include <algorithm>
#include <iterator>
#include <tuple>
#include <unordered_set>
#include <utility>
#endif

#include "geo.h"

namespace BusRoutes
{

using std::string_view_literals::operator""sv;

void Database::add_stop(std::string stop, Geo::Coordinate coord)
{
    stops_coords_[std::move(stop)] = coord;
}

std::optional<Geo::Coordinate> Database::get_stop_coorditate(const std::string& stop) const
{
    if (stops_coords_.count(stop) > 0)
        return stops_coords_.at(stop);

    return std::nullopt;
}

void Database::add_route(std::string bus, RouteInfoPtr route)
{
    bus_routes_[std::move(bus)] = std::move(route);
}

static double route_length(const std::vector<std::string>& stops,
                           const std::unordered_map<std::string, Geo::Coordinate>& stops_coords)
{
    double length = 0.0;

    for (auto it = stops.cbegin(); !stops.empty() && it != std::prev(stops.cend()); ++it)
        length += Geo::earth_distance(stops_coords.at(*it), stops_coords.at(*std::next(it)));

    return length;
}

ConstRouteInfoPtr Database::get_route(const std::string& bus) const
{
    if (bus_routes_.count(bus) == 0)
        return nullptr;

    const auto& route_info = bus_routes_.at(bus);

    if (!route_info->length.has_value())
        route_info->length = route_length(route_info->stops, stops_coords_);

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
        std::string_view lat_str = get_word(str, ","sv).first;
        std::string_view lon_str = get_word(str).first;
        double latitude = std::stod(std::string{lat_str.cbegin(), lat_str.cend()});
        double longitude = std::stod(std::string{lon_str.cbegin(), lon_str.cend()});
        return std::make_unique<AddStopRequest>(std::string{name.cbegin(), name.cend()},
                                                Geo::Coordinate{latitude, longitude});
    }
    else if (command == "Bus"sv)
    {
        constexpr unsigned MAX_STOPS_COUNT_IN_ROUTE = 100;

        auto route = std::make_shared<RouteInfo>();
        route->bus = {name.cbegin(), name.cend()};
        route->stops.reserve(MAX_STOPS_COUNT_IN_ROUTE);

        std::unordered_set<std::string_view> unique_stops;
        char stops_separator = '\0';

        while (!str.empty())
        {
            const auto [stop, separator] = get_word(str, "->"sv);
            unique_stops.insert(stop);
            if (stops_separator == '\0')
                stops_separator = separator;
            route->stops.emplace_back(stop.cbegin(), stop.cend());
        }

        route->unique_stops_count = unique_stops.size();

        if (stops_separator == '-')
            std::copy(std::next(route->stops.crbegin()), route->stops.crend(), std::back_inserter(route->stops));

        return std::make_unique<AddRouteRequest>(route->bus, std::move(route));
    }

    return nullptr;
}

AddStopRequest::AddStopRequest(std::string stop, Geo::Coordinate coord)
    : stop_(std::move(stop)), coord_(coord)
{
}

void AddStopRequest::execute(Database& db) const
{
    db.add_stop(std::move(stop_), coord_);
}

AddRouteRequest::AddRouteRequest(std::string bus, RouteInfoPtr route)
    : bus_(std::move(bus)), route_(route)
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

    if (command == "Bus"sv)
    {
        return std::make_unique<GetRouteRequest>(std::string{name.cbegin(), name.cend()});
    }

    return nullptr;
}

GetRouteRequest::GetRouteRequest(std::string bus)
    : bus_(std::move(bus))
{
}

Response GetRouteRequest::execute(const Database& db) const
{
    auto resp = db.get_route(bus_);

    if (!resp)
        resp = std::make_shared<RouteInfo>(RouteInfo{bus_});

    return resp;
}

} // namespace BusRoutes
