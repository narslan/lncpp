#include "ray.hpp"

namespace ln {
  ray::ray(vector _o, vector _d)
      : origin_{_o}
      , direction_{_d} {};

  vector ray::position(double t) const
  {
    return origin_ + (direction_ * t);
  }

} // namespace ln