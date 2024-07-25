#pragma once

#include "matrix.hpp"
namespace ln {
  class Scene;
  class Filter
  {
public:
    virtual std::pair<Vec3, bool> filter(const Vec3&) const = 0;
    virtual ~Filter() = default;
  };

  struct ClipFilter : public Filter
  {
    ClipFilter(matrix, Vec3, Scene&);
    matrix m_;
    Vec3 eye_;
    Scene& scene_;
    ~ClipFilter() override;
    std::pair<Vec3, bool> filter(const Vec3&) const override;
  };

} // namespace ln