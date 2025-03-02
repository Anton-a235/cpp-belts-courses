#pragma once

namespace Geo
{

// Constant π
constexpr double PI = 3.1415926535;

// The Earth radius in meters
constexpr double EARTH_RADIUS = 6'371'000.0;

struct Coordinate
{
    double latitude;
    double longitude;
};

double earth_distance(Coordinate from, Coordinate to);

} // namespace Geo
