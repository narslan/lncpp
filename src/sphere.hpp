#pragma once
#include "box.hpp"
#include "shape.hpp"
namespace ln {

  struct sphere : public Shape
  {
    sphere(vector, double);
    ~sphere() = default;

    vector center_;
    double radius_;
    box box_;

    void compile() override;
    box boundingBox() override;
    const bool contains(const vector&, double) override;
    const hit intersect(const ray&) override;
    Paths paths() override;
    unsigned Counter() override;
  };

} // namespace ln