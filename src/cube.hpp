#pragma once
#include "box.hpp"
#include "shape.hpp"

namespace ln {

  struct cube : public Shape
  {
    cube(Vec3 n, Vec3 x);
    ~cube() = default;

    Vec3 _min;
    Vec3 _max;
    box _b;
    unsigned _counter;
    void compile() override;
    box boundingBox() override;
    const bool contains(const Vec3&, double) override;
    const hit intersect(const ray&) override;
    Paths paths() override;
    unsigned Counter() override;
  };

} // namespace ln