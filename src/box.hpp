#pragma once
#include "axis.hpp"
#include "ray.hpp"
#include <memory>
#include <vector>
namespace ln {
  class Shape;
  struct box
  {
    box(vector min, vector max);
    box();
    vector size() const;
    vector anchor(const vector& anch) const;
    vector center() const;
    box extend(const box& b) const;
    std::pair<double, double> intersect(const ray& r) const;
    bool contains(const vector& vec) const;
    std::pair<bool, bool> partition(Axis ax, double point) const;
    ~box() = default;
    vector min_; //min
    vector max_; //max
  };
  box boxForShapes(std::vector<std::shared_ptr<Shape>>);

} // namespace ln