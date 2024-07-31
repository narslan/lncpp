#pragma once
#include "box.hpp"
#include "shape.hpp"
namespace ln {

  struct sphere : public Shape
  {
    sphere(Vec3, double);
    ~sphere() = default;

    Vec3 center_;
    double radius_;
    box box_;

    void compile() override;
    box boundingBox() override;
    const bool contains(const Vec3&, double) override;
    const hit intersect(const ray&) override;
    Paths paths() override;
  };

} // namespace ln