#pragma once

#include "box.hpp"
#include "filter.hpp"
#include "matrix.hpp"
#include "vec3.hpp"

namespace ln {
  class Paths;
  struct Path
  {
    Path(std::initializer_list<Vec3>);
    Path(const std::vector<Vec3>&&);
    Path& operator+=(const Path& rhs);
    Path& operator+=(const Vec3& rhs);
    ~Path() = default;

    box boundingBox() const;
    void print() const;
    std::string toSVG() const;
    Path transform(const matrix&) const;
    Path chop(double) const;
    Paths filter(Filter& f) const;
    Path simplify(double) const;

    std::vector<Vec3> p;
  };

} // namespace ln