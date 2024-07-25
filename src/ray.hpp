#pragma once
#include "vector.hpp"
namespace ln {
  struct ray
  {
public:
    ray(vector o, vector d);
    vector position(double t) const;

    ~ray() = default;
    vector origin_;
    vector direction_;
  };

} // namespace ln