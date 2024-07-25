#pragma once

#include "box.hpp"
#include "filter.hpp"
#include "matrix.hpp"
#include <vector>

namespace ln {
  class Paths;
  struct Path
  {
public:
    Path(std::initializer_list<vector>);
    Path(const std::vector<vector>&&);
    Path& operator+=(const Path& rhs);
    Path& operator+=(const vector& rhs);
    ~Path() = default;

    box boundingBox() const;
    void print() const;
    std::string toSVG() const;
    Path transform(const matrix&) const;
    Path chop(double) const;
    Paths filter(Filter& f) const;
    Path simplify(double) const;

    std::vector<vector> p;
  };

} // namespace ln