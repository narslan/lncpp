#include "cube.hpp"

namespace ln {

  cube::cube(vector n, vector x)
      : _min{n}
      , _max{x}
      , _b{n, x}
      , _counter{0} {};
  void cube::compile(){};
  box cube::boundingBox()
  {
    return _b;
  }
  const bool cube::contains(const vector& vec, double f)
  {
    auto v = vec.v_data;
    if((v[0] < (_min.v_data[0] - f)) || (v[0] > (_max.v_data[0] + f))) {
      return false;
    }
    if((v[1] < (_min.v_data[1] - f)) || (v[1] > (_max.v_data[1] + f))) {
      return false;
    }
    if((v[2] < (_min.v_data[2] - f)) || (v[2] > (_max.v_data[2] + f))) {
      return false;
    }
    return true;
  }

  const hit cube::intersect(const ray& r)
  {
    _counter++;
    auto n = (_min - r.origin_) / r.direction_;
    auto f = (_max - r.origin_) / r.direction_;

    n = n.min(f);
    f = n.max(f);
    auto nv = n.v_data;
    auto fv = f.v_data;
    auto t0 = std::max(std::max(nv[0], nv[1]), nv[2]);
    auto t1 = std::min(std::min(fv[0], fv[1]), fv[2]);

    if((t0 < 1e-3) && (t1 > 1e-3)) {
      return hit(t1);
    }

    if((t0 >= 1e-3) && (t0 < t1)) {
      return hit(t0);
    }

    return hit();
  }

  Paths cube::paths()
  {
    auto x1 = _min.v_data[0];
    auto y1 = _min.v_data[1];
    auto z1 = _min.v_data[2];
    auto x2 = _max.v_data[0];
    auto y2 = _max.v_data[1];
    auto z2 = _max.v_data[2];
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