#include "geo.h"

#ifndef IGNORE_STD_HEADERS
#include <cmath>
#endif

namespace Geo
{

double to_radians(double degrees)
{
    return degrees * PI / 180.0;
}

double earth_distance(Coordinate from, Coordinate to)
{
    auto [lat1, lon1] = from;
    auto [lat2, lon2] = to;

    // Преобразование координат в радианы
    double lat1_rad = to_radians(lat1);
    double lon1_rad = to_radians(lon1);
    double lat2_rad = to_radians(lat2);
    double lon2_rad = to_radians(lon2);

    // Разница широт и долгот
    double d_lat = lat2_rad - lat1_rad;
    double d_lon = lon2_rad - lon1_rad;

    // Вычисление по формуле гаверсинусов
    double a = std::sin(d_lat / 2) * std::sin(d_lat / 2) +
               std::cos(lat1_rad) * std::cos(lat2_rad) * std::sin(d_lon / 2) * std::sin(d_lon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    return EARTH_RADIUS * c;
}

} // namespace Geo
