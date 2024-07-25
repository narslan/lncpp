#pragma once
#include "vec3.hpp"
#include <vector>
namespace ln {
  double radians(double degree);
  double degrees(double radian);
  double median(std::vector<double>& v);
  const Vec3 latLngToXYZ(double latd, double lngd, double radius);
} // namespace ln