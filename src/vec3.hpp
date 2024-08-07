#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

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

    //Vec3al operations

    constexpr bool operator==(const Vec3& rhs) const
    {
      return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    constexpr Vec3 operator+(const Vec3& rhs) const
    {
      return Vec3{x + rhs.x, y + rhs.y, z + rhs.z};
    };

    constexpr Vec3 operator-(const Vec3& rhs) const
    {
      return Vec3{x - rhs.x, y - rhs.y, z - rhs.z};
    };

    constexpr Vec3 operator*(const Vec3& rhs) const
    {
      return Vec3{x * rhs.x, y * rhs.y, z * rhs.z};
    };

    constexpr Vec3 operator/(const Vec3& rhs) const
    {
      // if(std::any_of(rhs.v_data.begin(), rhs.v_data.end(), [](double i) { return i == 0.0; }))
      //   throw std::runtime_error("divide by zero");

      return Vec3{x / rhs.x, y / rhs.y, z / rhs.z};
    };

    constexpr double dot(const Vec3& rhs) const
    {
      return x * rhs.x + y * rhs.y + z * rhs.z;
    };
    Vec3 cross(const Vec3& rhs) const
    {
      auto a = y * rhs.z - z * rhs.y;
      auto b = z * rhs.x - x * rhs.z;
      auto c = x * rhs.y - y * rhs.x;
      return Vec3{a, b, c};
    };

    constexpr double segmentDistance(const Vec3& v, const Vec3& w) const
    {
      auto l2 = v.distanceSquared(w);

      if(l2 == 0.0) {
        return distance(v);
      }
      auto t = (*this - v).dot(w - v) / l2;
      if(t < 0.0) {
        return distance(v);
      }
      if(t > 1.0) {
        return distance(w);
      }
      return distance(v + ((w - v) * t));
    };
    constexpr Vec3 normalize() const
    {
      auto l = length();
      return Vec3{x / l, y / l, z / l};
    };

    constexpr double lengthSquared() const
    {
      return x * x + y * y + z * z;
    };
    constexpr double distance(const Vec3& rhs) const
    {
      return (*this - rhs).length();
    };

    constexpr double distanceSquared(const Vec3& rhs) const
    {
      return (*this - rhs).lengthSquared();
    };

    //Scalar ops
    constexpr Vec3 operator+(const double& rhs) const
    {
      return Vec3{x + rhs, y + rhs, z + rhs};
    };

    constexpr Vec3 operator-(const double& rhs) const
    {
      return Vec3{x - rhs, y - rhs, z - rhs};
    };

    constexpr Vec3 operator*(const double& rhs) const
    {
      return Vec3{x * rhs, y * rhs, z * rhs};
    };

    constexpr Vec3 operator/(const double& rhs) const
    {
      // if(rhs == 0.0)
      //   throw std::runtime_error("divide by zero");
      return Vec3{x / rhs, y / rhs, z / rhs};
    };

    constexpr Vec3 min(const Vec3& rhs) const
    {
      return Vec3{std::min(x, rhs.x), std::min(y, rhs.y), std::min(z, rhs.z)};
    };

    constexpr Vec3 max(const Vec3& rhs) const
    {
      return Vec3{std::max(x, rhs.x), std::max(y, rhs.y), std::max(z, rhs.z)};
    };

    constexpr double minComponent() const
    {

      return std::min(std::min(x, y), z);
    };

    constexpr Vec3 minAxis() const
    {
      auto a = std::abs(x);
      auto b = std::abs(y);
      auto c = std::abs(z);

      if(a <= b && a <= c) {
        return Vec3{1, 0, 0};
      }
      else if(b <= a && b <= c) {
        return Vec3{0, 1, 0};
      }
      return Vec3{0, 0, 1};
    };
    friend std::ostream& operator<<(std::ostream& stream, const Vec3& v)
    {
      return std::cout << "[" << v.x << ' ' << v.y << ' ' << v.z << "]";
    };
  };
  //This is an additional external vector constructor for making random vectors.
  inline Vec3 randomUnitVector()
  {
    std::random_device r;
    // Choose a random mean between 0.0 and 1.0 .
    std::default_random_engine e1(r());
    std::uniform_real_distribution<double> dist(1.0, 100.0);

    return Vec3{dist(r), dist(r), dist(r)}.normalize();
  };
} // namespace ln