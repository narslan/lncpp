#include "shape.hpp"

namespace ln {
  void EmptyShape::compile(){};
  box EmptyShape::boundingBox()
  {
    Vec3 v1{};
    Vec3 v2{};
    box b{v1, v2};
    return b;
  }

  const bool EmptyShape::contains(const Vec3& v, double f)
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

  EmptyShape::EmptyShape(){};
  EmptyShape::~EmptyShape(){};
} // namespace ln
