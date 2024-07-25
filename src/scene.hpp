#pragma once
#include "btree.hpp"
#include "shape.hpp"

namespace ln {
  class Scene
  {
public:
    Scene();
    Scene& operator=(Scene&&) = default;
    ~Scene() = default;

    void Compile();
    void Add(std::shared_ptr<Shape>);
    hit Intersect(const ray&) const;
    bool Visible(vector eye, vector point) const;
    Paths GetPaths() const;
    Paths Render(vector eye,
                 vector center,
                 vector up,
                 double width,
                 double height,
                 double fovy,
                 double near,
                 double far,
                 double step);
    Paths RenderWithMatrix(matrix m, vector eye, double width, double height, double step);

    unsigned Size();

private:
    std::vector<std::shared_ptr<Shape>> _shapes;
    BTree _tree;
  };

} // namespace ln
