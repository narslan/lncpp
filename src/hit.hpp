#pragma once

namespace ln {

  struct hit
  {
    double t;
    hit(double);
    // no hit.
    hit();
    hit& operator=(const hit& b);
    const hit& min(const hit& b) const;
    const hit& max(const hit& b) const;
    const bool ok() const;
    ~hit() = default;
  };
  static constexpr double INF = 1e+9;
  static constexpr double EPS = 1e-9;
} // namespace ln