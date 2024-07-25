#include "filter.hpp"
#include "box.hpp"
#include "scene.hpp"
#include <utility>

namespace ln {

  ClipFilter::ClipFilter(matrix m, vector v, Scene& s)
      : m_{m}
      , eye_{v}
      , scene_{s}
  { }

  ClipFilter::~ClipFilter(){};
  std::pair<vector, bool> ClipFilter::filter(const vector& v) const
  {

    const box clipBox = box{{-1, -1, -1}, {1, 1, 1}};

    auto w = m_.mulPositionW(v);
    if(!scene_.Visible(eye_, v)) {
      return std::pair<vector, bool>(w, false);
    }

    if(!clipBox.contains(w)) {
      return std::pair<vector, bool>(w, false);
    }
    return std::pair<vector, bool>(w, true);
  }

} // namespace ln