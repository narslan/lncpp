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

  // this is actually not median, but what go implementation had.
  double median(std::vector<double>& v)
  {
    auto n = v.size();

    if(n == 0) {
      return 0;
    }
    if(n % 2 == 1) {
      return v[n / 2];
    }
    else {
      auto a = v[(n / 2) - 1];
      auto b = v[(n / 2)];
      return (a + b) / 2;
    }
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