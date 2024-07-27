#include "box.hpp"
#include "shape.hpp"
#include <stdexcept>
#include <utility>
namespace ln {
  box::box(Vec3 min, Vec3 max)
      : _min(min)
      , _max(max){};

  box& box::operator=(const box& other)
  {
    // std::swap(_min, other._min);
    // std::swap(_max, other._max);
    _min = other._min;
    _max = other._max;
    return *this;
  } //copy assignment

  Vec3 box::size() const
  {
    return _max - _min;
  };

  Vec3 box::anchor(const Vec3& anch) const
  {
    return _min + this->size() * anch;
  };

  Vec3 box::center() const
  {
    return this->anchor(Vec3{0.5, 0.5, 0.5});
  };

  box box::extend(const box& b) const
  {
    auto mnn = _min.min(b._min);
    auto mxx = _max.max(b._max);
    return box{mnn, mxx};
  };

  bool box::contains(const Vec3& b) const
  {
    return _min.x <= b.x && _max.x >= b.x && _min.y <= b.y && _max.y >= b.y && _min.z <= b.z &&
           _max.z >= b.z;
  };

  std::pair<double, double> box::intersect(const ray& r) const
  {

    auto x1 = (_min.x - r._origin.x) / r._direction.x;
    auto y1 = (_min.y - r._origin.y) / r._direction.y;
    auto z1 = (_min.z - r._origin.z) / r._direction.z;
    auto x2 = (_max.x - r._origin.x) / r._direction.x;
    auto y2 = (_max.y - r._origin.y) / r._direction.y;
    auto z2 = (_max.z - r._origin.z) / r._direction.z;

    if(x1 > x2) {
      std::swap(x1, x2);
    }
    if(y1 > y2) {
      std::swap(y1, y2);
    }
    if(z1 > z2) {
      std::swap(z1, z2);
    }
    auto t1 = std::max(std::max(x1, y1), z1);
    auto t2 = std::min(std::min(x2, y2), z2);

    return std::pair<double, double>(t1, t2);
    // auto v1 = (_min - r._origin) / r._direction;
    // auto v2 = (_max - r._origin) / r._direction;
    // return v1.intersectMax(v2);
  };

  std::pair<bool, bool> box::partition(Axis ax, double point) const
  {
    bool left;
    bool right;
    switch(ax) {
    case AxisX /*case*/:
      left = _min.x <= point;
      right = _max.x >= point;
      break;
    case AxisY /*case*/:
      left = _min.y <= point;
      right = _max.y >= point;
      break;

    case AxisZ /*case*/:
      left = _min.z <= point;
      right = _max.z >= point;
      break;
    case AxisNone:
      std::runtime_error(" should not reach here AxisNone");
      break;
    }
    return std::pair<bool, bool>(left, right);
  };

  box boxForShapes(std::vector<std::shared_ptr<Shape>> shapes)
  {
    if(shapes.size() == 0) {
      return box{};
    }

    box b = shapes.front()->boundingBox();
    for(auto& s : shapes) {
      b = b.extend(s->boundingBox());
    }
    return b;
  }
} // namespace ln