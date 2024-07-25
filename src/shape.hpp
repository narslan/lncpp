#pragma once
#include "hit.hpp"
#include "paths.hpp"
namespace ln {

  class Shape
  {
public:
    virtual void compile() = 0;
    virtual box boundingBox() = 0;
    virtual const bool contains(const Vec3&, double) = 0;
    virtual const hit intersect(const ray&) = 0; //TODO: implement hit in place of bool
    virtual Paths paths() = 0;
    virtual unsigned Counter() = 0;
    virtual ~Shape() = default;
  };

  class EmptyShape : public Shape
  {
public:
    void compile() override;
    EmptyShape();
    ~EmptyShape() override;
    box boundingBox() override;
    const bool contains(const Vec3&, double) override;
    const hit intersect(const ray&) override;
    Paths paths() override;
    unsigned Counter() override;
  };

} // namespace ln
