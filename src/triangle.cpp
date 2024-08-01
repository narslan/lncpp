#include "triangle.hpp"
#include "box.hpp"

namespace ln {
  triangle::triangle(Vec3 v1, Vec3 v2, Vec3 v3)
      : e1{v1}
      , e2{v2}
      , e3{v3}
  {
    auto min = e1.min(e2).min(e3);
    auto max = e1.max(e2).max(v3);
    _box = box{min, max};
  }

  void triangle::compile(){};
  box triangle::boundingBox()
  {
    return _box;
  };
  const bool triangle::contains(const Vec3& v, double d)
  {
    return false;
  };
  const hit triangle::intersect(const ray& r)
  {
    double e1x = e2.x - e1.x;
    double e1y = e2.y - e1.y;
    double e1z = e2.z - e1.z;
    double e2x = e3.x - e1.x;
    double e2y = e3.y - e1.y;
    double e2z = e3.z - e1.z;

    auto px = r._direction.y * e2z - r._direction.z * e2y;
    auto py = r._direction.z * e2x - r._direction.x * e2z;
    auto pz = r._direction.x * e2y - r._direction.y * e2x;

    auto det = e1x * px + e1y * py + e1z * pz;
    if(det > -EPS && det < EPS) {
      return hit{};
    }
    auto inv = 1 / det;
    auto tx = r._origin.x - e1.x;
    auto ty = r._origin.y - e1.y;
    auto tz = r._origin.z - e1.z;

    auto u = (tx * px + ty * py + tz * pz) * inv;

    if(u < 0 || u > 1) {
      return hit{};
    }
    auto qx = ty * e1z - tz * e1y;
    auto qy = tz * e1x - tx * e1z;
    auto qz = tx * e1y - ty * e1x;
    auto v = (r._direction.x * qx + r._direction.y * qy + r._direction.z * qz) * inv;
    if(v < 0 || u + v > 1) {
      return hit{};
    }

    auto d = (e2x * qx + e2y * qy + e2z * qz) * inv;
    if(d < EPS) {
      return hit{};
    }

    return hit{d};
  };
  Paths triangle::paths()
  {
    return Paths{Path{e1, e2}, Path{e2, e3}, Path{e3, e1}};
  };

} // namespace ln