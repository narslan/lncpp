#include "shape.hpp"

namespace ln {
  void EmptyShape::compile(){};
  box EmptyShape::boundingBox()
  {
    vector v1{};
    vector v2{};
    box b{v1, v2};
    return b;
  }

  const bool EmptyShape::contains(const vector& v, double f)
  {
    return false;
  }

  const hit EmptyShape::intersect(const ray& r)
  {
    return hit();
  }
  Paths EmptyShape::paths()
  {
    return Paths{};
  };
  unsigned EmptyShape::Counter()
  {
    return 0;
  };
  EmptyShape::EmptyShape(){};
  EmptyShape::~EmptyShape(){};
} // namespace ln
