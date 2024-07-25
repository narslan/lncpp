#pragma once
#include "vector.hpp"
#include <vector>
namespace ln {
  double radians(double degree);
  double degrees(double radian);
  double median(std::vector<double>& v);
  const vector latLngToXYZ(double latd, double lngd, double radius);
} // namespace ln