#include "sphere.hpp"
#include "paths.hpp"
#include "util.hpp"
#include "vec3.hpp"
#include <cmath>

namespace ln {

  sphere::sphere(Vec3 center, double radius)
      : center_{center}
      , radius_{radius}
      , box_{center - radius, center + radius}
  { }

  void sphere::compile() {};
  box sphere::boundingBox()
  {
    return box_;
  };
  const bool sphere::contains(const Vec3& v, double f)
  {
    return (v - center_).length() <= radius_ + f;
  };

  const hit sphere::intersect(const ray& r)
  {
    auto radius = radius_;
    auto to = r._origin - center_;
    auto b = to.dot(r._direction);
    auto c = to.dot(to) - (radius * radius);
    auto d = (b * b) - c;
    if(d > 0) {
      d = std::sqrt(d);
      auto t1 = -b - d;
      if(t1 > 1e-2) {
        return hit{t1};
      }
      auto t2 = -b + d;
      if(t2 > 1e-2) {
        return hit{t2};
      }
    }
    return hit{};
  };

  Paths sphere::paths()
  {
    Paths paths{};
    auto n = 10.0;
    auto o = 10.0;

    for(double lat = -90 + o; lat <= 90 - o; lat += n) {
      Path path{};
      for(double lng = 0; lng <= 360; lng += n) {
        const Vec3 v = latLngToXYZ(lat, lng, radius_) + center_;
        path += v;
      }
      paths += path;
    }

    for(double lng = 0; lng <= 360; lng += n) {
      Path path{};
      for(double lat = -90 + o; lng <= 90 - o; lat++) {
        const Vec3 v = latLngToXYZ(lat, lng, radius_) + center_;
        path += v;
      }
      paths += path;
    }

    return paths;
  };

} // namespace ln