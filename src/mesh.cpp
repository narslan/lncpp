#include "mesh.hpp"
#include "btree.hpp"
#include "triangle.hpp"
#include <memory>

namespace ln {
  mesh::mesh(std::vector<triangle> trs)
      : _box{}
      , _triangles{trs}
      , _tree{}
  {
    if(_triangles.size() != 0) {
      _box = _triangles.front().boundingBox();
      for(auto el : _triangles) {
        _box = _box.extend(el.boundingBox());
      }
    }
  }

  void mesh::compile()
  {
    if(_tree.isEmpty()) {
      std::vector<std::shared_ptr<Shape>> shapes;
      for(auto el : _triangles) {
        auto tr = std::make_shared<triangle>(el);
        shapes.push_back(tr);
      }
      _tree = BTree{shapes};
    }
  };
  box mesh::boundingBox()
  {
    return _box;
  };

  const bool mesh::contains(const Vec3&, double)
  {
    return false;
  };
  const hit mesh::intersect(const ray& r)
  {
    return _tree.intersect(r);
  };
  Paths mesh::paths()
  {
    Paths result{};
    for(auto el : _triangles) {
      result += el.paths();
    }
    return result;
  };

} // namespace ln