#include "ray.hpp"

namespace ln {
  ray::ray(Vec3 o, Vec3 d)
      : _origin{o}
      , _direction{d} {};

  Vec3 ray::position(double t) const
  {
    return _origin + (_direction * t);
  }

} // namespace ln