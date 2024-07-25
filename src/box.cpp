#include "box.hpp"
#include "shape.hpp"
#include <stdexcept>
#include <utility>
namespace ln {
  box::box(vector min, vector max)
      : min_(min)
      , max_(max){};
  box::box()
      : min_{}
      , max_{} {};
  vector box::size() const
  {
    return max_ - min_;
  };

  vector box::anchor(const vector& anch) const
  {
    return min_ + this->size() * anch;
  };

  vector box::center() const
  {
    return this->anchor(vector{0.5, 0.5, 0.5});
  };

  box box::extend(const box& b) const
  {
    auto mnn = min_.min(b.min_);
    auto mxx = max_.max(b.max_);
    return box{mnn, mxx};
  };

  bool box::contains(const vector& vec) const
  {
    return ((vec.max(max_) == max_) && (vec.min(min_) == min_));
  };

  std::pair<double, double> box::intersect(const ray& r) const
  {

    auto v1 = (min_ - r.origin_) / r.direction_;
    auto v2 = (max_ - r.origin_) / r.direction_;
    return v1.intersectMax(v2);
  };

  std::pair<bool, bool> box::partition(Axis ax, double point) const
  {
    bool left;
    bool right;
    switch(ax) {
    case AxisX /*case*/:
      left = min_.v_data[0] <= point;
      right = max_.v_data[0] >= point;
      break;
    case AxisY /*case*/:
      left = min_.v_data[1] <= point;
      right = max_.v_data[1] >= point;
      break;

    case AxisZ /*case*/:
      left = min_.v_data[2] <= point;
      right = max_.v_data[2] >= point;
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