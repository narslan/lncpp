#include "btree.hpp"
#include "axis.hpp"
#include "box.hpp"
#include "util.hpp"
#include <algorithm>
#include <memory>
namespace ln {

  BNode::BNode(std::shared_ptr<BNode>& lft,
               std::vector<std::shared_ptr<Shape>> init,
               Axis axis,
               double point,
               std::shared_ptr<BNode>& rgt)
      : _lft{lft}
      , _shapes{init}
      , _axis{axis}
      , _point{point}
      , _rgt{rgt}
  { }

  BNode::BNode()
      : _lft{nullptr}
      , _shapes{}
      , _axis{AxisNone}
      , _point{}
      , _rgt{nullptr}
  { }

  // BNode& BNode::operator=(BNode other)
  // {
  //   std::swap(_lft, other._lft);
  //   std::swap(_shapes, other._shapes);
  //   std::swap(_axis, other._axis);
  //   std::swap(_point, other._point);
  //   std::swap(_rgt, other._rgt);
  //   return *this;
  // }

  BNode& BNode::operator=(const BNode& other)
  {
    _lft = other._lft;
    _shapes = other._shapes;
    _axis = other._axis;
    _point = other._point;
    _rgt = other._rgt;
    return *this;
  }

  hit BNode::intersect(const ray& r, double tmin, double tmax) const
  {

    double tsplit;
    bool leftFirst;

    if(_axis == AxisNone) {
      return intersectShapes(r);
    }
    if(_axis == AxisX) {
      tsplit = (_point - r._origin.x) / r._direction.x;
      leftFirst = (r._origin.x < _point) || ((r._origin.x == _point) && (r._direction.x <= 0));
    }
    if(_axis == AxisY) {
      //  std::cout << _point << ' ' << r._origin.y << ' ' << r._direction.y << std::endl;
      tsplit = (_point - r._origin.y) / r._direction.y;
      leftFirst = (r._origin.y < _point) || ((r._origin.y == _point) && (r._direction.y <= 0));
    }
    if(_axis == AxisZ) {
      tsplit = (_point - r._origin.z) / r._direction.z;
      leftFirst = (r._origin.z < _point) || ((r._origin.z == _point) && (r._direction.z <= 0));
    }

    BNode first{};
    BNode second{};

    if(leftFirst) {
      first = *_lft;
      second = *_rgt;
    }
    else {
      first = *_rgt;
      second = *_lft;
    }
    if((tsplit > tmax) || (tsplit <= 0)) {
      return first.intersect(r, tmin, tmax);
    }
    else if(tsplit < tmin) {
      return second.intersect(r, tmin, tmax);
    }
    else {
      auto h1 = first.intersect(r, tmin, tsplit);
      if(h1.t <= tsplit) {
        return h1;
      }
      auto h2 = second.intersect(r, tsplit, std::min(tmax, h1.t));
      if(h1.t <= h2.t) {
        return h1;
      }
      else {
        return h2;
      }
    }
  };

  hit BNode::intersectShapes(const ray& r) const
  {
    hit h{};

    for(auto& shape : _shapes) {
      auto hs = shape->intersect(r);
      if(hs.t < h.t) {
        h = hs;
      }
    }
    return h;
  };

  BTree& BTree::operator=(BTree other)
  {
    std::swap(_box, other._box);
    std::swap(_root, other._root);
    return *this;
  }

  BTree::BTree(std::vector<std::shared_ptr<Shape>> shapes)
  {
    _box = boxForShapes(shapes);
    auto l = std::make_shared<BNode>();
    auto r = std::make_shared<BNode>();
    auto b = BNode{l, shapes, AxisNone, 0, r};
    _root = std::make_shared<BNode>(b);

    _root->split(0);
  }

  void BNode::split(int depth)
  {

    auto s = _shapes.size();
    if(s < 8)
      return;
    std::vector<double> xs;
    std::vector<double> ys;
    std::vector<double> zs;

    for(auto shape : _shapes) {
      auto b = shape->boundingBox();

      xs.push_back(b._min.x);
      xs.push_back(b._max.x);
      ys.push_back(b._min.y);
      ys.push_back(b._max.y);
      zs.push_back(b._min.z);
      zs.push_back(b._max.z);
    }

    std::sort(xs.begin(), xs.end());
    std::sort(ys.begin(), ys.end());
    std::sort(zs.begin(), zs.end());

    auto mx = median(xs);
    auto my = median(ys);
    auto mz = median(zs);
    //fmt::print("median mx: {}\n", mx);
    auto best = static_cast<int>(s * 0.85);

    auto bestAxis = AxisNone;
    auto bestPoint = 0.0;
    auto sx = partitionScore(AxisX, mx);
    if(sx < best) {
      best = sx;
      bestAxis = AxisX;
      bestPoint = mx;
    }

    auto sy = partitionScore(AxisY, my);
    if(sy < best) {
      best = sy;
      bestAxis = AxisY;
      bestPoint = my;
    }

    auto sz = partitionScore(AxisZ, mz);
    if(sz < best) {
      best = sz;
      bestAxis = AxisZ;
      bestPoint = mz;
    }

    if(bestAxis == AxisNone) {
      return;
    }

    auto lr = partition(best, bestAxis, bestPoint);
    _axis = bestAxis;
    _point = bestPoint;

    auto l1 = std::make_shared<BNode>();
    auto r1 = std::make_shared<BNode>();
    auto l2 = std::make_shared<BNode>();
    auto r2 = std::make_shared<BNode>();
    auto n1 = BNode{l1, lr.first, AxisNone, 0, r1};
    auto n2 = BNode{l2, lr.second, AxisNone, 0, r2};

    _lft = std::make_shared<BNode>(n1);
    _rgt = std::make_shared<BNode>(n2);

    _lft->split(depth + 1);
    _rgt->split(depth + 1);
  }

  std::pair<std::vector<std::shared_ptr<Shape>>, std::vector<std::shared_ptr<Shape>>>
  BNode::partition(int size, Axis axis, double point) const
  {

    std::vector<std::shared_ptr<Shape>> left;
    std::vector<std::shared_ptr<Shape>> right;

    for(auto& s : _shapes) {
      auto b = s->boundingBox();
      auto lr = b.partition(axis, point);
      if(lr.first) {
        left.push_back(s);
      }
      if(lr.second) {
        right.push_back(s);
      }
    }

    return std::pair<std::vector<std::shared_ptr<Shape>>, std::vector<std::shared_ptr<Shape>>>(
        left, right);
  };

  int BNode::partitionScore(Axis ax, double point) const
  {

    int left = 0;
    int right = 0;

    for(auto& shape : _shapes) {
      auto b = shape->boundingBox();
      auto lr = b.partition(ax, point);
      if(lr.first) {
        left++;
      }
      if(lr.second) {
        right++;
      }
    }

    if(left >= right) {
      return left;
    }
    else {
      return right;
    }
  }

  hit BTree::intersect(const ray& r) const
  {
    auto tlr = _box.intersect(r);

    if((tlr.second < tlr.first) || (tlr.second <= 0)) {
      return hit{};
    }

    return _root->intersect(r, tlr.first, tlr.second);
  };

  bool BTree::isEmpty() const
  {
    return !_root;
  }
} // namespace ln