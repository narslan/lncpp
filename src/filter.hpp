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
    ClipFilter(const matrix&, const Vec3&, const Scene&);
    matrix _m;
    Vec3 _eye;
    const Scene& _scene;
    ~ClipFilter() override;
    std::pair<Vec3, bool> filter(const Vec3&) const override;
  };

} // namespace ln