#include "filter.hpp"
#include "box.hpp"
#include "scene.hpp"
#include <utility>

namespace ln {

  ClipFilter::ClipFilter(matrix m, Vec3 v, Scene& s)
      : m_{m}
      , eye_{v}
      , scene_{s}
  { }

  ClipFilter::~ClipFilter(){};
  std::pair<Vec3, bool> ClipFilter::filter(const Vec3& v) const
  {

    const box clipBox = box{{-1, -1, -1}, {1, 1, 1}};

    auto w = m_.mulPositionW(v);
    if(!scene_.Visible(eye_, v)) {
      return std::pair<Vec3, bool>(w, false);
    }

    if(!clipBox.contains(w)) {
      return std::pair<Vec3, bool>(w, false);
    }
    return std::pair<Vec3, bool>(w, true);
  }

} // namespace ln