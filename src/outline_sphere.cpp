#include "outline_sphere.hpp"
#include "util.hpp"
#include <cmath>
#include <iostream>
namespace ln {
  outlineSphere::outlineSphere(vector eye, vector up, vector center, double radius)
      : eye_{eye}
      , up_{up}
      , sphere{center, radius}
  { }
  Paths outlineSphere::paths()
  {
    Path path{};
    auto center = center_;
    auto radius = radius_;

    auto hyp = (center - eye_).length();
    auto opp = radius;
    auto theta = std::asin(opp / hyp);
    auto adj = opp / std::tan(theta);
    auto d = std::cos(theta) * adj;
    auto r = std::sin(theta) * adj;

    auto w = (center - eye_).normalize();
    auto u = w.cross(up_).normalize();
    auto v = w.cross(u).normalize();
    auto c = eye_ + (w * d);

    for(double i = 0; i <= 360; i++) {
      double a = radians(i);
      auto p = c;
      p = p + (u * std::cos(a) * r);
      p = p + (v * std::sin(a) * r);

      path += p;
    }
    return Paths{path};
  }
} // namespace ln