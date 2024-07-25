#include "util.hpp"
#include "vec3.hpp"
#include <algorithm>
#include <cmath>

namespace ln {

  double radians(double degree)
  {
    return degree * std::numbers::pi / 180;
  }

  double degrees(double radian)
  {
    return radian * 180 / std::numbers::pi;
  }

  double median(std::vector<double>& v)
  {
    size_t n = v.size() / 2;
    nth_element(v.begin(), v.begin() + n, v.end());
    return v[n];
  }

  const Vec3 latLngToXYZ(double latd, double lngd, double radius)
  {

    auto lat = radians(latd);
    auto lng = radians(lngd);
    auto x = radius * std::cos(lat) * std::cos(lng);
    auto y = radius * std::cos(lat) * std::sin(lng);
    auto z = radius * std::sin(lat);
    return Vec3{x, y, z};
  }
} // namespace ln