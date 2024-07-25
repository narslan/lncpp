
#include "paths.hpp"

#include <format>
#include <iostream>
namespace ln {
  Paths::Paths(std::initializer_list<Path> _paths)
      : ps_{_paths}
  { }

  Paths& Paths::operator+=(const Paths& rhs)
  {
    for(auto path : rhs.ps_) {
      this->ps_.push_back(path);
    }
    return *(this);
  };

  Paths& Paths::operator+=(const Path& rhs)
  {
    this->ps_.push_back(rhs);
    return *(this);
  };

  box Paths::boundingBox() const
  {
    auto b = ps_.front().boundingBox();
    for(auto p : ps_) {
      b = b.extend(p.boundingBox());
    }
    return b;
  }
  void Paths::print() const
  {
    for(auto p : ps_) {
      p.print();
    }
  }

  Paths Paths::transform(const matrix& m) const
  {
    Paths result{};
    for(auto p : ps_) {
      result += p.transform(m);
    }
    return result;
  };
  Paths Paths::chop(double step) const
  {
    Paths result{};
    for(auto p : ps_) {
      result += p.chop(step);
    }
    return result;
  };

  Paths Paths::filter(Filter& f) const
  {
    Paths result{};
    for(auto p : ps_) {
      result += p.filter(f);
    }
    return result;
  };

  Paths Paths::simplify(double threshold) const
  {
    Paths result{};
    for(auto p : ps_) {
      result += p.simplify(threshold);
    }
    return result;
  };

  unsigned Paths::size() const
  {
    return ps_.size();
  }

  std::string Paths::toSVG(double width, double height) const
  {
    std::string result;
    result += std::format("<svg width=\"{}\" height=\"{}\" version=\"1.1\" "
                          "baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\">\n",
                          width,
                          height);

    result += std::format("<g transform=\"translate(0,{}) scale(1,-1)\">\n", height);
    for(auto p : ps_) {
      result += p.toSVG();
      result += '\n';
    }
    result += "</g></svg>\n";
    return result;
  }
} // namespace ln