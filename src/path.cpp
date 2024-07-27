#include "path.hpp"
#include "paths.hpp"

#include <format>
#include <iostream>

namespace ln {
  Path::Path(std::initializer_list<Vec3> _vectors)
      : p{_vectors}
  { }

  Path::Path(const std::vector<Vec3>&& other)
  {

    p = std::move(other);
  }

  Path& Path::operator+=(const Path& rhs)
  {
    for(auto el : rhs.p) {
      p.push_back(el);
    }
    return *(this);
  };

  Path& Path::operator+=(const Vec3& rhs)
  {
    p.push_back(rhs);
    return *(this);
  };

  box Path::boundingBox() const
  {
    auto b = box{p[0], p[0]};
    for(auto v : p) {
      b = b.extend(box{v, v});
    }
    return b;
  }
  void Path::print() const
  {
    for(auto v : p) {
      std::cout << std::format("{:.6f},{:.6f};", v.x, v.y) << '\n';
    }
  }

  std::string Path::toSVG() const
  {
    const int precision{2};
    const int width{15};
    std::string points;
    auto svg = [](const Vec3& v) { return std::format("{:.6f},{:.6f}", v.x, v.y); };
    for(auto el : p) {
      points += svg(el);
      points += ' ';
    }

    return std::format("<polyline stroke=\"black\" fill=\"none\" points=\"{}\"/>", points);
  }

  Path Path::transform(const matrix& m) const
  {
    std::vector<Vec3> tv;
    auto mul = [&m](const Vec3& v) { return m.mulPosition(v); };
    for(auto el : p) {
      tv.push_back(mul(el));
    }
    return Path{std::move(tv)};
  }

  Path Path::chop(double step) const
  {
    std::vector<Vec3> result;
    for(int i = 0; i < p.size() - 1; i++) {
      auto a = p[i];
      auto b = p[i + 1];
      auto v = b - a;
      auto l = v.length();
      if(i == 0) {
        result.push_back(a);
      }
      auto d = step;
      while(d < l) {
        result.push_back(a + (v * (d / l)));
        d += step;
      }
      result.push_back(b);
    }
    return Path{std::move(result)};
  }

  Paths Path::filter(Filter& f) const
  {
    Paths paths_result{};
    Path path{};

    for(auto el : p) {
      //std::cout << "[ " << v.x << ' ' << v.x << ' ' << v.x << ' ' << " ]\n";
      // std::cout << "{" << el.x << ' ' << el.y << ' ' << el.z << "}\n";
      auto lr = f.filter(el);

      if(lr.second) {
        //std::cout << "ok" << std::endl;
        path += lr.first;
      }
      else {

        if(path.p.size() > 1) {
          paths_result += path;
        }
        path.p.clear();
      }
    }

    if(path.p.size() > 1) {
      paths_result += path;
    }
    return paths_result;
  };

  Path Path::simplify(double threshold) const
  {
    if(p.size() < 3) {
      return *this;
    }

    auto a = p.front();
    auto b = p.back();
    auto index = -1;
    auto distance = 0.0;
    // std::cout << p.size() << '\n';
    for(int i = 1; i < p.size() - 1; i++) {
      auto d = p[i].segmentDistance(a, b);
      if(d > distance) {
        index = i;
        distance = d;
        //std::cout << std::format("{},{:.8e};", i, d) << '\n';
      }
    }

    if(distance > threshold) {
      auto v1 = std::vector<Vec3>(p.begin(), p.begin() + index + 1);
      auto p1 = Path{std::move(v1)};
      auto r1 = p1.simplify(threshold);

      auto v2 = std::vector<Vec3>(p.begin() + index, p.end());
      auto p2 = Path{std::move(v2)};
      auto r2 = p2.simplify(threshold);

      auto res = std::vector<Vec3>(r1.p.begin(), r1.p.end() - 1);
      auto p3 = Path{std::move(res)};
      p3 += r2;

      return p3;
    }
    else {
      return Path{a, b};
    }
  }

} // namespace ln