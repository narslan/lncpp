#include "cube.hpp"

namespace ln {

  cube::cube(Vec3 min, Vec3 max)
      : _min{min}
      , _max{max}
      , _b{min, max}
      , _counter{0} {};
  void cube::compile(){};

  box cube::boundingBox()
  {
    //std::cout << "[ " << _b._min.x << ' ' << _b._min.y << ' ' << _b._min.z << " ]"
    //          << "[ " << _b._max.x << ' ' << _b._max.y << ' ' << _b._max.z << " ]\n";
    return _b;
  }
  const bool cube::contains(const Vec3& v, double f)
  {

    if((v.x < (_min.x - f)) || (v.x > (_max.x + f))) {
      return false;
    }
    if((v.y < (_min.y - f)) || (v.y > (_max.y + f))) {
      return false;
    }
    if((v.z < (_min.z - f)) || (v.z > (_max.z + f))) {
      return false;
    }
    return true;
  }

  const hit cube::intersect(const ray& r)
  {
    _counter++;
    auto n = (_min - r._origin) / r._direction;
    auto f = (_max - r._origin) / r._direction;

    n = n.min(f);
    f = n.max(f);

    auto t0 = std::max(std::max(n.x, n.y), n.z);
    auto t1 = std::min(std::min(f.x, f.y), f.z);

    if((t0 < 1e-3) && (t1 > 1e-3)) {
      return hit{t1};
    }

    if((t0 >= 1e-3) && (t0 < t1)) {
      return hit{t0};
    }

    return hit{};
  }

  Paths cube::paths()
  {
    auto x1 = _min.x;
    auto y1 = _min.y;
    auto z1 = _min.z;
    auto x2 = _max.x;
    auto y2 = _max.y;
    auto z2 = _max.z;
    return Paths{{{x1, y1, z1}, {x1, y1, z2}},
                 {{x1, y1, z1}, {x1, y2, z1}},
                 {{x1, y1, z1}, {x2, y1, z1}},
                 {{x1, y1, z2}, {x1, y2, z2}},
                 {{x1, y1, z2}, {x2, y1, z2}},
                 {{x1, y2, z1}, {x1, y2, z2}},
                 {{x1, y2, z1}, {x2, y2, z1}},
                 {{x1, y2, z2}, {x2, y2, z2}},
                 {{x2, y1, z1}, {x2, y1, z2}},
                 {{x2, y1, z1}, {x2, y2, z1}},
                 {{x2, y1, z2}, {x2, y2, z2}},
                 {{x2, y2, z1}, {x2, y2, z2}}};
  };

  unsigned cube::Counter()
  {
    return _counter;
  };
} // namespace ln