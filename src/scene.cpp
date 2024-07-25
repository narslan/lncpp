#include "scene.hpp"
#include "matrix.hpp"
#include <iostream>
#include <memory>

namespace ln {

  Scene::Scene()
      : _shapes{}
      , _tree{}
  { }

  void Scene::Add(std::shared_ptr<Shape> sh)
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

  unsigned Scene::Size()
  {
    return _shapes.size();
  };

  hit Scene::Intersect(const ray& r) const
  {
    return _tree.intersect(r);
  }
  bool Scene::Visible(vector eye, vector point) const
  {
    auto v = eye - point;
    auto r = ray{point, v.normalize()};

    auto h = Intersect(r);

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

  Paths Scene::Render(vector eye,
                      vector center,
                      vector up,
                      double width,
                      double height,
                      double fovy,
                      double near,
                      double far,
                      double step)
  {
    double aspect = double(width * 1.0 / height);
    auto m = lookAt(eye, center, up);
    auto m2 = m.perspective(fovy, aspect, near, far);
    return RenderWithMatrix(m2, eye, width, height, step);
  };

  Paths Scene::RenderWithMatrix(matrix m, vector eye, double width, double height, double step)
  {
    Compile();
    auto ps = GetPaths();
    if(step > 0) {
      ps = ps.chop(step);
    }

    ClipFilter c{m, eye, *this};
    ps = ps.filter(c);
    // for(auto p : ps.ps_) {
    //   p.print();
    // }

    if(step > 0) {
      ps = ps.simplify(1e-6);
    }
    m = translatev({1, 1, 0}).scale({width / 2, height / 2, 0});
    ps = ps.transform(m);
    // for(auto s : _shapes) {
    //   std::cout << s->Counter() << '\n';
    // }
    return ps;
  };

} // namespace ln