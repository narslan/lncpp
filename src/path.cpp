#include "path.hpp"
#include "paths.hpp"
#include <format>
#include <iostream>
namespace ln {
  Path::Path(std::initializer_list<vector> _vectors)
      : p{_vectors}
  { }

  Path::Path(const std::vector<vector>&& other)
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

  Path& Path::operator+=(const vector& rhs)
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
      std::cout << std::format("{:.6f},{:.6f};", v.v_data[0], v.v_data[1]) << '\n';
    }
  }

  std::string Path::toSVG() const
  {
    const int precision{2};
    const int width{15};
    std::string points;
    auto svg = [](const vector& v) {
      return std::format("{:.6f},{:.6f}", v.v_data[0], v.v_data[1]);
    };
    for(auto el : p) {
      points += svg(el);
      points += ' ';
    }

    return std::format("<polyline stroke=\"black\" fill=\"none\" points=\"{}\"/>", points);
  }

  Path Path::transform(const matrix& m) const
  {
    std::vector<vector> tv;
    auto mul = [&m](const vector& v) { return m.mulPosition(v); };
    for(auto el : p) {
      tv.push_back(mul(el));
    }
    return Path{std::move(tv)};
  }

  Path Path::chop(double step) const
  {
    std::vector<vector> result;
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

    for(std::size_t i = 0; auto el : p) {
      auto lr = f.filter(el);

      if(lr.second) {
        path.p.push_back(lr.first);
      }
      else {

        if(path.p.size() > 1) {
          paths_result += path;
        }
        path.p.clear();
      }
      ++i;
    }

    if(path.p.size() > 1) {
      paths_result += path;
    }
    return paths_result;
  };

  Path Path::simplify(double threshold) const
  {
    if(p.size() < 3) {
      return *(this);
    }

    auto a = p.front();
    auto b = p.back();
    auto index = -1;
    auto distance = 0.0;

    for(int i = 0; i < p.size(); i++) {
      auto d = p[i].segmentDistance(a, b);
      if(d > distance) {
        index = i;
        distance = d;
      }
    }

    if(distance > threshold) {
      auto v1 = std::vector<vector>(p.begin(), p.begin() + index + 1);
      auto p1 = Path{std::move(v1)};
      auto r1 = p1.simplify(threshold);

      auto v2 = std::vector<vector>(p.begin() + index, p.end());
      auto p2 = Path{std::move(v2)};
      auto r2 = p2.simplify(threshold);

      auto res = std::vector<vector>(r1.p.begin(), r1.p.end() - 1);
      auto p3 = Path{std::move(res)};
      p3 += r2;
      return p3;
    }
    else {
      return Path{a, b};
    }
  }

} // namespace ln