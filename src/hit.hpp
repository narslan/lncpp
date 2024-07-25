#pragma once

namespace ln {

  struct hit
  {
    double t;
    hit(double);
    // no hit.
    hit();
    const hit& min(const hit& b) const;
    const hit& max(const hit& b) const;
    const bool ok() const;
    ~hit() = default;
  };
  static constexpr double INF = 1e+9;
} // namespace ln