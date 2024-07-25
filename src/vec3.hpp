#pragma once

#include <cmath>
namespace ln {

  struct Vec3
  {

    double x, y, z;
    constexpr Vec3() noexcept = default;
    constexpr Vec3(const Vec3& other) noexcept = default;
    constexpr Vec3(const double& v0, const double& v1, const double& v2) noexcept
        : x{v0}
        , y{v1}
        , z{v2}
    { }
    constexpr Vec3& operator=(const Vec3& other) noexcept = default;

    //length and distance
    constexpr double length() const
    {
      return std::sqrt(x * x + y * y + z * z);
    };

    //linear algebra
    constexpr Vec3 normalize() const
    {
      auto l = length();
      return Vec3{x / l, y / l, z / l};
    };
  };

} // namespace ln