#include "vector.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <format>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>
namespace ln {

  vector vector::normalize() const
  {
    double l = length();
    return vector{v_data[0] / l, v_data[1] / l, v_data[2] / l};
  };

  std::string vector::print() const
  {
    std::string s = "[";
    for(const double& el : v_data) {
      s += std::format(" {:.2f} ", el);
    }
    s += "]";
    return s;
  };

  std::ostream& operator<<(std::ostream& stream, const vector& vector)
  {
    return std::cout << vector.print();
  };

  vector vector::operator+(const vector& rhs) const
  {
    return vector{v_data[0] + rhs.v_data[0], v_data[1] + rhs.v_data[1], v_data[2] + rhs.v_data[2]};
  };

  vector vector::operator*(const vector& rhs) const
  {
    return vector{v_data[0] * rhs.v_data[0], v_data[1] * rhs.v_data[1], v_data[2] * rhs.v_data[2]};
  };

  vector vector::operator/(const vector& rhs) const
  {
    // if(std::any_of(rhs.v_data.begin(), rhs.v_data.end(), [](double i) { return i == 0.0; }))
    //   throw std::runtime_error("divide by zero");

    return vector{v_data[0] / rhs.v_data[0], v_data[1] / rhs.v_data[1], v_data[2] / rhs.v_data[2]};
  };

  bool vector::operator==(const vector& rhs) const
  {
    return v_data == rhs.v_data;
  };

  vector vector::cross(const vector& rhs) const
  {
    auto x = v_data[1] * rhs.v_data[2] - v_data[2] * rhs.v_data[1];
    auto y = v_data[2] * rhs.v_data[0] - v_data[0] * rhs.v_data[2];
    auto z = v_data[0] * rhs.v_data[1] - v_data[1] * rhs.v_data[0];
    return vector{x, y, z};
  };

  double vector::segmentDistance(const vector& rhsv, const vector& rhsw) const
  {
    auto l2 = rhsv.distanceSquared(rhsw);
    if(l2 == 0.0) {
      return distance(rhsv);
    }
    auto t = (*this - rhsv).dot(rhsw - rhsv) / l2;
    if(t < 0.0) {
      return distance(rhsv);
    }
    if(t > 1.0) {
      return distance(rhsw);
    }
    return distance(rhsv + ((rhsw - rhsv) * t));
  };

  vector vector::operator+(const double& rhs) const
  {
    return vector{v_data[0] + rhs, v_data[1] + rhs, v_data[2] + rhs};
  };

  vector vector::operator*(const double& rhs) const
  {
    return vector{v_data[0] * rhs, v_data[1] * rhs, v_data[2] * rhs};
  };

  vector vector::operator/(const double& rhs) const
  {
    if(rhs == 0.0)
      throw std::runtime_error("divide by zero");
    return vector{v_data[0] / rhs, v_data[1] / rhs, v_data[2] / rhs};
  };

  vector vector::min(const vector& rhs) const
  {
    return vector{std::min(v_data[0], rhs.v_data[0]),
                  std::min(v_data[1], rhs.v_data[1]),
                  std::min(v_data[2], rhs.v_data[2])};
  };

  vector vector::max(const vector& rhs) const
  {
    return vector{std::max(v_data[0], rhs.v_data[0]),
                  std::max(v_data[1], rhs.v_data[1]),
                  std::max(v_data[2], rhs.v_data[2])};
  };

  std::pair<double, double> vector::intersectMax(const vector& rhs) const
  {
    //TODO: need fix here.
    double x1{v_data[0]};
    double x2{rhs.v_data[0]};
    double y1{v_data[1]};
    double y2{rhs.v_data[1]};
    double z1{v_data[2]};
    double z2{rhs.v_data[2]};

    if(x1 > x2) {
      std::swap(x1, x2);
    }
    if(y1 > y2) {
      std::swap(y1, y2);
    }
    if(z1 > z2) {
      std::swap(z1, z2);
    }

    return std::pair<double, double>(std::max({x1, y1, z1}), std::min({x2, y2, z2}));
  };

  double vector::minComponent() const
  {
    auto result = std::min_element(v_data.begin(), v_data.end());
    return *result;
  };

  vector vector::minAxis() const
  {
    auto x = std::abs(v_data[0]);
    auto y = std::abs(v_data[1]);
    auto z = std::abs(v_data[2]);

    if(x <= y && x <= z) {
      return vector{1, 0, 0};
    }
    else if(y <= x && y <= z) {
      return vector{0, 1, 0};
    }
    return vector{0, 0, 1};
  };

  //This is an additional external vector constructor for making random vectors.
  vector randomUnitVector()
  {
    std::random_device r;
    // Choose a random mean between 0.0 and 1.0 .
    std::default_random_engine e1(r());
    std::uniform_real_distribution<double> dist(1.0, 100.0);

    return vector{dist(r), dist(r), dist(r)}.normalize();
  };

} // namespace ln
