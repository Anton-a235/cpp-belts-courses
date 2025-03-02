#pragma once

#ifndef IGNORE_STD_HEADERS
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>
#endif

#include "geo.h"

namespace Transport
{

struct StopInfo
{
    std::optional<Geo::Coordinate> coordinate;
    std::set<std::string> buses;
};

struct RouteInfo
{
    std::vector<std::string> stops;
    std::optional<double> length;
    int unique_stops_count;
};

class Database
{
public:
    void add_stop(std::string stop, Geo::Coordinate coord);
    const StopInfo& get_stop(const std::string& stop) const;

    void add_route(std::string bus, RouteInfo route);
    const RouteInfo& get_route(const std::string& bus) const;

private:
    std::unordered_map<std::string, StopInfo> stops_;
    mutable std::unordered_map<std::string, RouteInfo> routes_;
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
    AddRouteRequest(std::string bus, RouteInfo route);

    void execute(Database& db) const override;

private:
    std::string bus_;
    RouteInfo route_;
};

class ReadRequest;
using ReadRequestPtr = std::unique_ptr<ReadRequest>;

struct GetStopResponse
{
    std::string stop;
    const StopInfo& info;

    GetStopResponse() = delete;
};

struct GetRouteResponse
{
    std::string bus;
    const RouteInfo& route;

    GetRouteResponse() = delete;
};

using Response = std::variant<std::monostate, GetStopResponse, GetRouteResponse>;

class ReadRequest
{
public:
    virtual ~ReadRequest() = default;

    static ReadRequestPtr from_string(std::string_view str);

    virtual Response execute(const Database& db) const = 0;
};

class GetStopRequest : public ReadRequest
{
public:
    GetStopRequest(std::string stop);

    Response execute(const Database& db) const override;

private:
    std::string stop_;
};

class GetRouteRequest : public ReadRequest
{
public:
    GetRouteRequest(std::string bus);

    Response execute(const Database& db) const override;

private:
    std::string bus_;
};

} // namespace Transport
