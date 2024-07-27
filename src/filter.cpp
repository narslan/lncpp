#include "filter.hpp"
#include "box.hpp"
#include "scene.hpp"
#include <utility>

namespace ln {

  ClipFilter::ClipFilter(const matrix& m, const Vec3& v, const Scene& s)
      : _m{m}
      , _eye{v}
      , _scene{s}
  { }

  ClipFilter::~ClipFilter(){};
  std::pair<Vec3, bool> ClipFilter::filter(const Vec3& v) const
  {

    const box clipBox = box{{-1, -1, -1}, {1, 1, 1}};
    //std::cout << "[ " << _eye.x << ' ' << _eye.y << ' ' << _eye.z << ' ' << " ]\n";
    // std::cout << "[ " << v.x << ' ' << v.y << ' ' << v.z << ' ' << " ]\n";
    auto w = _m.mulPositionW(v);
    //std::cout << "[ " << w.x << ' ' << w.y << ' ' << w.z << ' ' << " ]\n";
    if(!(_scene.Visible(_eye, v))) {
      //  std::cout << "[ " << w.x << ' ' << w.y << ' ' << w.z << " ]\n";
      return std::pair<Vec3, bool>(w, false);
    }

    if(!(clipBox.contains(w))) {
      //std::cout << "[ " << w.x << ' ' << w.x << ' ' << w.x << ' ' << " ]\n";
      return std::pair<Vec3, bool>(w, false);
    }
    return std::pair<Vec3, bool>(w, true);
  }

} // namespace ln