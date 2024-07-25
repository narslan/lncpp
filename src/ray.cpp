#include "ray.hpp"

namespace ln {
  ray::ray(Vec3 _o, Vec3 _d)
      : origin_{_o}
      , direction_{_d} {};

  Vec3 ray::position(double t) const
  {
    return origin_ + (direction_ * t);
  }

} // namespace ln