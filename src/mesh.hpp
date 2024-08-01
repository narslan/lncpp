#pragma once
#include "btree.hpp"
#include "shape.hpp"
#include "triangle.hpp"
namespace ln {

  struct mesh : public Shape
  {
    mesh(std::vector<triangle>);
    ~mesh() = default;

    void compile() override;
    box boundingBox() override;
    const bool contains(const Vec3&, double) override;
    const hit intersect(const ray&) override;
    Paths paths() override;

    box _box;
    BTree _tree;
    std::vector<triangle> _triangles;
  };
} // namespace ln