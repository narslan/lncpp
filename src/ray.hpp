#pragma once
#include "vec3.hpp"
namespace ln {
  struct ray
  {
public:
    ray(Vec3 o, Vec3 d);
    Vec3 position(double t) const;

    ~ray() = default;
    Vec3 _origin;
    Vec3 _direction;
  };

} // namespace ln