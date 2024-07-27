#include "scene.hpp"
#include "matrix.hpp"
#include <iostream>
#include <memory>

namespace ln {

  Scene::Scene()
      : _shapes{}
      , _tree{}
  { }

  void Scene::Add(const std::shared_ptr<Shape>& sh)
  {
    _shapes.push_back(sh);
  };

  void Scene::Compile()
  {
    for(auto& shape : _shapes) {
      shape->compile();
    }

    if(_tree.isEmpty()) {
      _tree = BTree{_shapes};
    }
  };

  unsigned Scene::Size() const
  {
    return _shapes.size();
  };

  hit Scene::Intersect(const ray& r) const
  {
    return _tree.intersect(r);
  }
  bool Scene::Visible(const Vec3& eye, const Vec3& point) const
  {
    auto v = eye - point;
    auto r = ray{point, v.normalize()};
    // std::cout << v.normalize() << point << std::endl;
    auto h = this->Intersect(r);

    return h.t >= v.length();
  }
  Paths Scene::GetPaths() const
  {
    Paths result{};
    for(auto& shape : _shapes) {
      result += shape->paths();
    }

    return result;
  };

  Paths Scene::Render(Vec3 eye,
                      Vec3 center,
                      Vec3 up,
                      double width,
                      double height,
                      double fovy,
                      double near,
                      double far,
                      double step)
  {
    double aspect = width / height;
    auto m = lookAt(eye, center, up);
    auto m2 = m.perspective(fovy, aspect, near, far);
    return RenderWithMatrix(m2, eye, width, height, step);
  };

  Paths Scene::RenderWithMatrix(matrix m, Vec3 eye, double width, double height, double step)
  {
    Compile();
    auto pts = GetPaths();
    if(step > 0) {
      pts = pts.chop(step);
    }

    ClipFilter c{m, eye, *this};
    pts = pts.filter(c);
    //std::cout << pts.size() << std::endl;

    // for(auto p : ps.ps_) {
    //   p.print();
    // }

    if(step > 0) {
      pts = pts.simplify(1e-6);
    }

    Vec3 v{width / 2, height / 2, 0};
    m = translatev({1, 1, 0}).scale(v);

    // for(auto s : _shapes) {
    //   std::cout << s->Counter() << '\n';
    // }
    return pts.transform(m);
  };

} // namespace ln
