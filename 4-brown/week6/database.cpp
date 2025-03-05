#include "database.h"

#ifndef IGNORE_STD_HEADERS
#include <algorithm>
#include <charconv>
#include <exception>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#endif

#include "geo.h"

namespace Transport
{

using std::string_view_literals::operator""sv;

Database::Database(const Json::Node& routing_settings)
{
    const auto& settings = routing_settings.AsMap();
    bus_wait_time_ = settings.at("bus_wait_time").AsDouble();
    bus_velocity_ = settings.at("bus_velocity").AsDouble() * 1000.0 / 60.0; // m per min
}

void Database::add_stop(std::string_view stop,
                        Geo::Coordinate coord,
                        std::unordered_map<std::string_view, int> distances)
{
    road_distances_[stop].merge(std::move(distances));
    stops_[stop].coordinate = coord;
}

const StopInfo& Database::get_stop(std::string_view stop) const
{
    static const StopInfo empty_info{};
    return stops_.count(stop) > 0 ? stops_.at(stop) : empty_info;
}

void Database::add_route(std::string_view bus, BusRouteInfo route)
{
    for (const auto& stop : route.stops)
        stops_[stop].buses.insert(bus);

    bus_routes_[bus] = std::move(route);
}

static double route_geo_length(const std::vector<std::string_view>& stops,
                               const std::unordered_map<std::string_view, StopInfo>& stops_info)
{
    double length = 0.0;

    for (auto it = stops.cbegin(); !stops.empty() && it != std::prev(stops.cend()); ++it)
        length += Geo::earth_distance(*stops_info.at(*it).coordinate, *stops_info.at(*std::next(it)).coordinate);

    return length;
}

static int get_road_distance(std::string_view from,
                             std::string_view to,
                             const Database::DistancesTable& road_distances_)
{
    if (road_distances_.count(from) > 0 && road_distances_.at(from).count(to) > 0)
        return road_distances_.at(from).at(to);
    else
        return road_distances_.at(to).at(from);
}

static int route_road_length(const std::vector<std::string_view>& stops,
                             const Database::DistancesTable& road_distances_)
{
    int length = 0;

    for (auto it = stops.cbegin(); !stops.empty() && it != std::prev(stops.cend()); ++it)
        length += get_road_distance(*it, *std::next(it), road_distances_);

    return length;
}

const BusRouteInfo& Database::get_bus_route(std::string_view bus) const
{
    static const BusRouteInfo empty_info{};

    if (bus_routes_.count(bus) == 0)
        return empty_info;

    auto& route_info = bus_routes_.at(bus);

    if (!route_info.length.has_value())
    {
        route_info.length = route_road_length(route_info.stops, road_distances_);
        route_info.curvature = *route_info.length / route_geo_length(route_info.stops, stops_);
    }

    return route_info;
}

RouteInfo Database::get_route(std::string_view from, std::string_view to) const
{
    if (from == to)
        return {0.0};

    if (!routing_.has_value())
    {
        routing_.emplace();
        routing_->routes_graph = [this]()
        {
            Graph::DirectedWeightedGraph<double> graph(stops_.size());
            Graph::VertexId vertex_id{};

            for (const auto& [stop, _] : stops_)
                routing_->stop_vertex_ids[stop] = vertex_id++;

            for (const auto& [bus, bus_route_info] : bus_routes_)
            {
                const auto& bus_stops = bus_route_info.stops;

                for (size_t bus_stop_i = 0; bus_stop_i < bus_stops.size() - 1; ++bus_stop_i)
                {
                    double dist = 0.0;

                    for (size_t bus_stop_j = bus_stop_i + 1; bus_stop_j < bus_stops.size(); ++bus_stop_j)
                    {
                        dist += get_road_distance(bus_stops[bus_stop_j - 1], bus_stops[bus_stop_j], road_distances_);
                        RouteWaitItem wait_item{bus_stops[bus_stop_i], bus_wait_time_};
                        RouteBusItem bus_item{bus, bus_stop_j - bus_stop_i, dist / bus_velocity_};
                        const auto id_from = routing_->stop_vertex_ids.at(bus_stops[bus_stop_i]);
                        const auto id_to = routing_->stop_vertex_ids.at(bus_stops[bus_stop_j]);
                        Graph::Edge<double> new_edge{id_from, id_to, wait_item.time + bus_item.time};
                        const auto edge_id = graph.AddEdge(new_edge);
                        routing_->edges[edge_id] = {std::move(wait_item), std::move(bus_item)};
                    }
                }
            }

            return graph;
        }();
        routing_->router.emplace(*routing_->routes_graph);
    }

    const auto from_id = routing_->stop_vertex_ids.at(from);
    const auto to_id = routing_->stop_vertex_ids.at(to);
    const auto route_info = routing_->router->BuildRoute(from_id, to_id);

    if (!route_info.has_value())
        return {std::nullopt};

    RouteInfo result{route_info->weight};
    std::optional<RouteBusItem> bus_span = std::nullopt;

    for (size_t i = 0; i < route_info->edge_count; ++i)
    {
        const auto& edge = routing_->edges.at(routing_->router->GetRouteEdge(route_info->id, i));
        result.items.push_back(edge.first);
        result.items.push_back(edge.second);
    }

    routing_->router->ReleaseRoute(route_info->id);
    return result;
}

WriteRequestPtr WriteRequest::from_json(const Json::Node& node)
{
    const auto& req = node.AsMap();

    if (req.at("type").AsString() == "Stop"sv)
    {
        std::unordered_map<std::string_view, int> distances;

        for (const auto& [stop_to, dist] : req.at("road_distances").AsMap())
            distances[stop_to] = dist.AsInt();

        double latitude = req.at("latitude").AsDouble();
        double longitude = req.at("longitude").AsDouble();

        return std::make_unique<AddStopRequest>(
            req.at("name").AsString(), Geo::Coordinate{latitude, longitude}, std::move(distances));
    }
    else if (req.at("type").AsString() == "Bus"sv)
    {
        constexpr unsigned MAX_STOPS_COUNT_IN_ROUTE = 100;

        BusRouteInfo route{};
        route.stops.reserve(MAX_STOPS_COUNT_IN_ROUTE);

        std::unordered_set<std::string_view> unique_stops;

        for (const auto& stop : req.at("stops").AsArray())
        {
            route.stops.push_back(stop.AsString());
            unique_stops.insert(stop.AsString());
        }

        route.unique_stops_count = unique_stops.size();

        if (!req.at("is_roundtrip").AsBool())
            std::copy(std::next(route.stops.crbegin()), route.stops.crend(), std::back_inserter(route.stops));

        return std::make_unique<AddRouteRequest>(req.at("name").AsString(), std::move(route));
    }

    return nullptr;
}

AddStopRequest::AddStopRequest(std::string_view stop,
                               Geo::Coordinate coord,
                               std::unordered_map<std::string_view, int> distances)
    : stop_(stop), coord_(coord), distances_(std::move(distances))
{
}

void AddStopRequest::execute(Database& db) const
{
    db.add_stop(stop_, coord_, std::move(distances_));
}

AddRouteRequest::AddRouteRequest(std::string_view bus, BusRouteInfo route)
    : bus_(bus), route_(std::move(route))
{
}

void AddRouteRequest::execute(Database& db) const
{
    db.add_route(bus_, std::move(route_));
}

ReadRequest::ReadRequest(std::optional<int> request_id)
    : id_(request_id)
{
}

ReadRequestPtr ReadRequest::from_json(const Json::Node& node)
{
    const auto& req = node.AsMap();

    if (req.at("type").AsString() == "Stop"sv)
        return std::make_unique<GetStopRequest>(req.at("name").AsString(), req.at("id").AsInt());
    else if (req.at("type").AsString() == "Bus"sv)
        return std::make_unique<GetBusRequest>(req.at("name").AsString(), req.at("id").AsInt());
    else if (req.at("type").AsString() == "Route"sv)
        return std::make_unique<GetRouteRequest>(
            req.at("from").AsString(), req.at("to").AsString(), req.at("id").AsInt());

    return nullptr;
}

Transport::GetStopRequest::GetStopRequest(std::string_view stop, std::optional<int> request_id)
    : ReadRequest(request_id), stop_(stop)
{
}

Response GetStopRequest::execute(const Database& db) const
{
    return GetStopResponse{id_, db.get_stop(stop_)};
}

GetBusRequest::GetBusRequest(std::string_view bus, std::optional<int> request_id)
    : ReadRequest(request_id), bus_(bus)
{
}

Response GetBusRequest::execute(const Database& db) const
{
    return GetBusResponse{id_, db.get_bus_route(bus_)};
}

GetRouteRequest::GetRouteRequest(std::string_view stop_from, std::string_view stop_to, std::optional<int> request_id)
    : ReadRequest(request_id), stop_from_(stop_from), stop_to_(stop_to)
{
}

Response GetRouteRequest::execute(const Database& db) const
{
    return GetRouteResponse{id_, db.get_route(stop_from_, stop_to_)};
}

} // namespace Transport
