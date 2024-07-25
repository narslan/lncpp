#pragma once

#include <array>
#include <cmath>
#include <ostream>
#include <utility>
namespace ln {

  struct vector
  {
    constexpr vector(){};

    constexpr vector(double x, double y, double z)
        : v_data{x, y, z} {};

    vector normalize() const;
    std::string print() const;
    friend std::ostream& operator<<(std::ostream& stream, const vector& vector);

    double segmentDistance(const vector& rhs, const vector& rhs2) const;
    //vectoral operations
    vector operator+(const vector& rhs) const;
    constexpr vector operator-(const vector& rhs) const
    {
      return vector{
          v_data[0] - rhs.v_data[0], v_data[1] - rhs.v_data[1], v_data[2] - rhs.v_data[2]};
    };
    vector operator*(const vector& rhs) const;
    vector operator/(const vector& rhs) const;
    constexpr double dot(const vector& rhs) const
    {
      return v_data[0] * rhs.v_data[0] + v_data[1] * rhs.v_data[1] + v_data[2] * rhs.v_data[2];
    };
    vector cross(const vector& rhs) const;
    bool operator==(const vector& rhs) const;

    //scalar operations
    vector operator+(const double& rhs) const;
    constexpr vector operator-(const double& rhs) const
    {
      return vector{v_data[0] - rhs, v_data[1] - rhs, v_data[2] - rhs};
    };

    vector operator*(const double& rhs) const;
    vector operator/(const double& rhs) const;

    vector min(const vector& rhs) const;
    vector max(const vector& rhs) const;
    std::pair<double, double> intersectMax(const vector& rhs) const;

    double minComponent() const;
    vector minAxis() const;

    constexpr double length() const
    {
      return std::sqrt(v_data[0] * v_data[0] + v_data[1] * v_data[1] + v_data[2] * v_data[2]);
    };

    constexpr double lengthSquared() const
    {
      return v_data[0] * v_data[0] + v_data[1] * v_data[1] + v_data[2] * v_data[2];
    };
    constexpr double distance(const vector& rhs) const
    {
      return (*this - rhs).length();
    };

    constexpr double distanceSquared(const vector& rhs) const
    {
      return (*this - rhs).lengthSquared();
    };

    // matrix translate() const;
    // matrix scale() const;

    ~vector() = default;

    std::array<double, 3> v_data;
  };

  vector randomUnitVector();
} // namespace ln