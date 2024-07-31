#pragma once
#include "shape.hpp"
namespace ln {

  struct triangle : public Shape
  {
    triangle(Vec3, Vec3, Vec3, box);
    ~triangle() = default;

    Vec3 e1;
    Vec3 e2;
    Vec3 e3;
    box _box;

    void compile() override;
    box boundingBox() override;
    const bool contains(const Vec3&, double) override;
    const hit intersect(const ray&) override;
    Paths paths() override;
  };
} // namespace ln