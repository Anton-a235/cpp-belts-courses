#pragma once

#ifndef IGNORE_STD_HEADERS
#include <memory>
#include <optional>
#include <set>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>
#endif

#include "geo.h"
#include "json.h"
#include "router.h"

namespace Transport
{

struct StopInfo
{
    std::optional<Geo::Coordinate> coordinate;
    std::set<std::string_view> buses;
};

struct BusRouteInfo
{
    std::vector<std::string_view> stops;
    std::optional<int> length;
    std::optional<double> curvature;
    int unique_stops_count;
};

struct RouteWaitItem
{
    std::string_view stop_name;
    double time;
};

struct RouteBusItem
{
    std::string_view bus;
    size_t span_count;
    double time;
};

using RouteItem = std::variant<RouteWaitItem, RouteBusItem>;

struct RouteInfo
{
    std::optional<double> total_time;
    std::vector<RouteItem> items;
};

class Database
{
public:
    using DistancesTable = std::unordered_map<std::string_view, std::unordered_map<std::string_view, int>>;

    Database(const Json::Node& routing_settings);

    void add_stop(std::string_view stop, Geo::Coordinate coord, std::unordered_map<std::string_view, int> distances);
    const StopInfo& get_stop(std::string_view stop) const;

    void add_route(std::string_view bus, BusRouteInfo route);
    const BusRouteInfo& get_bus_route(std::string_view bus) const;
    RouteInfo get_route(std::string_view from, std::string_view to) const;

private:
    struct RoutingData
    {
        std::optional<Graph::DirectedWeightedGraph<double>> routes_graph;
        std::optional<Graph::Router<double>> router;
        std::unordered_map<std::string_view, Graph::VertexId> stop_vertex_ids;
        std::unordered_map<Graph::EdgeId, std::pair<RouteWaitItem, RouteBusItem>> edges;
    };

    DistancesTable road_distances_;
    std::unordered_map<std::string_view, StopInfo> stops_;
    mutable std::unordered_map<std::string_view, BusRouteInfo> bus_routes_;
    mutable std::optional<RoutingData> routing_;
    double bus_wait_time_;
    double bus_velocity_;
};

class WriteRequest;
using WriteRequestPtr = std::unique_ptr<WriteRequest>;

class WriteRequest
{
public:
    virtual ~WriteRequest() = default;

    static WriteRequestPtr from_json(const Json::Node& node);

    virtual void execute(Database& db) const = 0;
};

class AddStopRequest : public WriteRequest
{
public:
    AddStopRequest(std::string_view stop, Geo::Coordinate coord, std::unordered_map<std::string_view, int> distances);

    void execute(Database& db) const override;

private:
    std::string_view stop_;
    Geo::Coordinate coord_;
    std::unordered_map<std::string_view, int> distances_;
};

class AddRouteRequest : public WriteRequest
{
public:
    AddRouteRequest(std::string_view bus, BusRouteInfo route);

    void execute(Database& db) const override;

private:
    std::string_view bus_;
    BusRouteInfo route_;
};

class ReadRequest;
using ReadRequestPtr = std::unique_ptr<ReadRequest>;

struct GetStopResponse
{
    std::optional<int> request_id;
    const StopInfo& info;

    GetStopResponse() = delete;
};

struct GetBusResponse
{
    std::optional<int> request_id;
    const BusRouteInfo& route;

    GetBusResponse() = delete;
};

struct GetRouteResponse
{
    std::optional<int> request_id;
    RouteInfo route;
};

using Response = std::variant<std::monostate, GetStopResponse, GetBusResponse, GetRouteResponse>;

class ReadRequest
{
public:
    virtual ~ReadRequest() = default;

    ReadRequest(std::optional<int> request_id);

    static ReadRequestPtr from_json(const Json::Node& node);

    virtual Response execute(const Database& db) const = 0;

protected:
    std::optional<int> id_;
};

class GetStopRequest : public ReadRequest
{
public:
    GetStopRequest(std::string_view stop, std::optional<int> request_id = std::nullopt);

    Response execute(const Database& db) const override;

private:
    std::string_view stop_;
};

class GetBusRequest : public ReadRequest
{
public:
    GetBusRequest(std::string_view bus, std::optional<int> request_id = std::nullopt);

    Response execute(const Database& db) const override;

private:
    std::string_view bus_;
};

class GetRouteRequest : public ReadRequest
{
public:
    GetRouteRequest(std::string_view stop_from, std::string_view stop_to, std::optional<int> request_id = std::nullopt);

    Response execute(const Database& db) const override;

private:
    std::string_view stop_from_;
    std::string_view stop_to_;
};

} // namespace Transport
