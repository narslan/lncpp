#include "hit.hpp"
namespace ln {
  hit::hit(double _t)
      : t{_t} {};
  hit::hit()
      : t{INF} {};

  hit& hit::operator=(const hit& other)
  {
    t = other.t;
    return *this;
  }
  const hit& hit::min(const hit& b) const
  {
    if(t <= b.t) {
      return *this;
    }
    return b;
  }

  const hit& hit::max(const hit& b) const
  {
    if(t > b.t) {
      return *this;
    }
    return b;
  }

  const bool hit::ok() const
  {
    return t < INF;
  }

} // namespace ln