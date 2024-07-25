#pragma once
#include "box.hpp"
#include "shape.hpp"

namespace ln {

  struct cube : public Shape
  {
    cube(vector n, vector x);
    ~cube() = default;

    vector _min;
    vector _max;
    box _b;
    unsigned _counter;
    void compile() override;
    box boundingBox() override;
    const bool contains(const vector&, double) override;
    const hit intersect(const ray&) override;
    Paths paths() override;
    unsigned Counter() override;
  };

} // namespace ln