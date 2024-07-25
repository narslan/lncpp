#pragma once

#include "path.hpp"

namespace ln {

  struct Paths
  {
    Paths(std::initializer_list<Path>);
    Paths& operator+=(const Paths& rhs);
    Paths& operator+=(const Path& rhs);
    ~Paths() = default;
    box boundingBox() const;
    void print() const;
    unsigned size() const;
    Paths transform(const matrix&) const;
    Paths chop(double) const;
    Paths filter(Filter& f) const;
    Paths simplify(double) const;

    std::string toSVG(double width, double height) const;
    std::vector<Path> ps_;
  };
} // namespace ln
