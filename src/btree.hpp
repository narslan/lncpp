#pragma once
#include "hit.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include <initializer_list>
#include <memory>
namespace ln {
  struct BNode
  {
    BNode(std::shared_ptr<BNode>& lft,
          std::vector<std::shared_ptr<Shape>> shapes,
          Axis,
          double,
          std::shared_ptr<BNode>& rgt);
    BNode();
    ~BNode() = default;
    BNode& operator=(BNode other);

    std::shared_ptr<BNode> _lft;
    std::vector<std::shared_ptr<Shape>> _shapes;
    std::shared_ptr<BNode> _rgt;
    Axis _axis;
    double _point;
    //functions.
    void split(int depth);
    int partitionScore(Axis, double) const;
    std::pair<std::vector<std::shared_ptr<Shape>>, std::vector<std::shared_ptr<Shape>>>
    partition(int size, Axis axis, double point) const;
    hit intersect(const ray&, double, double) const;
    hit intersectShapes(const ray&) const;
    // static size_t howMany();

    // static size_t count;
  };

  class BTree
  {

public:
    explicit BTree(std::shared_ptr<BNode>& BNode);
    BTree(BTree&, std::initializer_list<std::shared_ptr<Shape>>, Axis, double, BTree&);
    BTree(std::initializer_list<std::shared_ptr<Shape>> init);
    BTree(std::vector<std::shared_ptr<Shape>> init);
    BTree();
    ~BTree() = default;
    BTree& operator=(BTree other);
    hit intersect(const ray& r) const;
    bool isEmpty() const;

private:
    std::shared_ptr<BNode> _root;
    box _box;
  };

} // namespace ln