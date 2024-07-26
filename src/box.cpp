#include "box.hpp"
#include "shape.hpp"
#include <stdexcept>
#include <utility>
namespace ln {
  box::box(Vec3 min, Vec3 max)
      : _min(min)
      , _max(max){};
  box::box()
      : _min{}
      , _max{} {};

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

  bool box::contains(const Vec3& vec) const
  {
    return ((vec.max(_max) == _max) && (vec.min(_min) == _min));
  };

  std::pair<double, double> box::intersect(const ray& r) const
  {

    auto v1 = (_min - r.origin_) / r.direction_;
    auto v2 = (_max - r.origin_) / r.direction_;
    return v1.intersectMax(v2);
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