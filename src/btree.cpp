#include "btree.hpp"
#include "axis.hpp"
#include "box.hpp"
#include "util.hpp"
#include <algorithm>
#include <iostream>
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

  // size_t BNode::howMany()
  // {
  //   return count;
  // }
  BNode::BNode()
      : _lft{nullptr}
      , _shapes{nullptr}
      , _axis{AxisNone}
      , _point{0}
      , _rgt{nullptr}
  { }

  BNode& BNode::operator=(BNode other)
  {
    std::swap(_lft, other._lft);
    std::swap(_shapes, other._shapes);
    std::swap(_axis, other._axis);
    std::swap(_point, other._point);
    std::swap(_rgt, other._rgt);
    return *this;
  }
  hit BNode::intersect(const ray& r, double tmin, double tmax) const
  {
    double tsplit;
    bool leftFirst;
    auto org = r.origin_;
    auto dir = r.direction_;
    switch(_axis) {
    case AxisNone:
      return intersectShapes(r);
    case AxisX:
      tsplit = (_point - org.x) / dir.x;
      leftFirst = (org.x < _point) || (org.x == _point && dir.x <= 0);
    case AxisY:
      tsplit = (_point - org.y) / dir.y;
      leftFirst = (org.y < _point) || (org.y == _point && dir.y <= 0);
    case AxisZ:
      tsplit = (_point - org.z) / dir.z;
      leftFirst = (org.z < _point) || (org.z == _point && dir.z <= 0);
    }

    // std::shared_ptr<BNode> first = std::make_shared<BNode>();
    // std::shared_ptr<BNode> second = std::make_shared<BNode>();
    //TODO: the number of lines should be minimized.
    // They are so many as to avoid copying shared pointers.
    if(leftFirst) {
      // first = _lft;
      // second = _rgt;
      if(tsplit > tmax || tsplit <= 0) {
        return _lft->intersect(r, tmin, tmax);
      }
      else if(tsplit < tmin) {
        return _rgt->intersect(r, tmin, tmax);
      }
      else {
        auto h1 = _lft->intersect(r, tmin, tmax);
        if(h1.t <= tsplit) {
          return h1;
        }
        auto h2 = _rgt->intersect(r, tsplit, std::min(tmax, h1.t));
        if(h1.t <= h2.t) {
          return h1;
        }
        else {
          return h2;
        }
      }
    }
    else {
      // first = _rgt;
      // second = _lft;

      if(tsplit > tmax || tsplit <= 0) {
        return _rgt->intersect(r, tmin, tmax);
      }
      else if(tsplit < tmin) {
        return _lft->intersect(r, tmin, tmax);
      }
      else {
        auto h1 = _rgt->intersect(r, tmin, tmax);
        if(h1.t <= tsplit) {
          return h1;
        }
        auto h2 = _lft->intersect(r, tsplit, std::min(tmax, h1.t));
        if(h1.t <= h2.t) {
          return h1;
        }
        else {
          return h2;
        }
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

  BTree::BTree(std::shared_ptr<BNode>& node)
      : _root(node)
  { }
  BTree::BTree(BTree& lft,
               std::initializer_list<std::shared_ptr<Shape>> init,
               Axis axis,
               double point,
               BTree& rgt)
      : _root(std::make_shared<BNode>(lft._root, init, axis, point, rgt._root))
  { }
  BTree::BTree() { }
  BTree::BTree(std::initializer_list<std::shared_ptr<Shape>> init) { }

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
      xs.push_back(b.min_.x);
      xs.push_back(b.max_.x);
      ys.push_back(b.min_.y);
      ys.push_back(b.max_.y);
      zs.push_back(b.min_.z);
      zs.push_back(b.max_.z);
    }
    //TODO: check if we need sorting
    std::sort(xs.begin(), xs.end());
    std::sort(ys.begin(), ys.end());
    std::sort(ys.begin(), ys.end());
    auto mxs = median(xs);
    auto mys = median(ys);
    auto mzs = median(zs);

    auto best = static_cast<int>(s * 0.85);

    auto bestAxis = AxisNone;
    auto bestPoint = 0.0;
    // std::cout << "mxs: " << mxs << "mys: " << mys << "mzs: " << mzs << "best: " << best
    //           << std::endl;
    auto sx = partitionScore(AxisX, mxs);
    if(sx < best) {
      best = sx;
      bestAxis = AxisX;
      bestPoint = mxs;
    }

    auto sy = partitionScore(AxisY, mys);
    if(sy < best) {
      best = sy;
      bestAxis = AxisY;
      bestPoint = mys;
    }

    auto sz = partitionScore(AxisZ, mzs);
    if(sz < best) {
      best = sz;
      bestAxis = AxisZ;
      bestPoint = mzs;
    }

    if(bestAxis == AxisNone) {
      return;
    }

    auto lr = partition(best, bestAxis, bestPoint);
    _axis = bestAxis;
    _point = bestPoint;

    auto l1 = std::make_shared<BNode>(BNode{});
    auto r1 = std::make_shared<BNode>(BNode{});
    auto l2 = std::make_shared<BNode>(BNode{});
    auto r2 = std::make_shared<BNode>(BNode{});
    auto n1 = BNode{l1, lr.first, AxisNone, 0, r1};
    auto n2 = BNode{l2, lr.first, AxisNone, 0, r2};

    _lft = std::move(std::make_shared<BNode>(n1));
    _rgt = std::move(std::make_shared<BNode>(n2));

    _lft->split(depth + 1);
    _rgt->split(depth + 1);
    _shapes.clear();
    //std::cout << depth << std::endl;
    //In the go implementation there was a line that nullifies shapes member.
    // With a comment "only needed at leaf nodes"
    //this->_shapes = nullptr; //nullify at leaf nodes
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

    for(auto shape : _shapes) {
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