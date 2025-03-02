#pragma once

#ifndef IGNORE_STD_HEADERS
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>
#endif

#include "geo.h"

namespace BusRoutes
{

struct RouteInfo
{
    std::string bus;
    std::vector<std::string> stops;
    std::optional<double> length;
    int unique_stops_count;
};

using RouteInfoPtr = std::shared_ptr<RouteInfo>;
using ConstRouteInfoPtr = std::shared_ptr<const RouteInfo>;

class Database
{
public:
    void add_stop(std::string stop, Geo::Coordinate coord);
    std::optional<Geo::Coordinate> get_stop_coorditate(const std::string& stop) const;

    void add_route(std::string bus, RouteInfoPtr route);
    ConstRouteInfoPtr get_route(const std::string& bus) const;

private:
    std::unordered_map<std::string, Geo::Coordinate> stops_coords_;
    std::unordered_map<std::string, RouteInfoPtr> bus_routes_;
};

class WriteRequest;
using WriteRequestPtr = std::unique_ptr<WriteRequest>;

class WriteRequest
{
public:
    virtual ~WriteRequest() = default;

    static WriteRequestPtr from_string(std::string_view str);

    virtual void execute(Database& db) const = 0;
};

class AddStopRequest : public WriteRequest
{
public:
    AddStopRequest(std::string stop, Geo::Coordinate coord);

    void execute(Database& db) const override;

private:
    std::string stop_;
    Geo::Coordinate coord_;
};

class AddRouteRequest : public WriteRequest
{
public:
    AddRouteRequest(std::string bus, RouteInfoPtr route);

    void execute(Database& db) const override;

private:
    std::string bus_;
    RouteInfoPtr route_;
};

class ReadRequest;
using ReadRequestPtr = std::unique_ptr<ReadRequest>;

using Response = std::variant<std::monostate, ConstRouteInfoPtr>;

class ReadRequest
{
public:
    virtual ~ReadRequest() = default;

    static ReadRequestPtr from_string(std::string_view str);

    virtual Response execute(const Database& db) const = 0;
};

class GetRouteRequest : public ReadRequest
{
public:
    GetRouteRequest(std::string bus);

    Response execute(const Database& db) const override;

private:
    std::string bus_;
};

} // namespace BusRoutes
