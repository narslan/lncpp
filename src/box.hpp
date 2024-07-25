#pragma once
#include "axis.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <memory>
#include <vector>
namespace ln {
  class Shape;
  struct box
  {
    box(Vec3 min, Vec3 max);
    box();
    Vec3 size() const;
    Vec3 anchor(const Vec3& anch) const;
    Vec3 center() const;
    box extend(const box& b) const;
    std::pair<double, double> intersect(const ray& r) const;
    bool contains(const Vec3& vec) const;
    std::pair<bool, bool> partition(Axis ax, double point) const;
    ~box() = default;
    Vec3 min_; //min
    Vec3 max_; //max
  };
  box boxForShapes(std::vector<std::shared_ptr<Shape>>);

} // namespace ln