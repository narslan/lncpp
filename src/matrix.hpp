#pragma once
#include "vec3.hpp"
#include <array>
#include <ostream>
namespace ln {
  struct matrix
  {

    matrix();
    explicit matrix(std::array<double, 16>&&);
    explicit matrix(const double&);
    ~matrix() = default;

    //operations on Vec3 and matrices
    Vec3 mulPosition(const Vec3&) const;
    Vec3 mulPositionW(const Vec3&) const;
    double determinant() const;
    matrix inverse() const;
    matrix transpose() const;
    matrix operator*(const matrix& rhs) const;

    matrix translate(const Vec3&) const;
    matrix scale(const Vec3&) const;
    matrix rotate(const Vec3&, double) const;
    matrix frustum(double, double, double, double, double, double) const;
    matrix ortographic(double, double, double, double, double, double) const;
    matrix perspective(double, double, double, double) const;
    //utility functions
    std::string print() const;
    double getElement(int, int) const;
    friend std::ostream& operator<<(std::ostream& stream, const matrix& matrix);

    std::array<double, 16> m_data;
  };
  matrix random_matrix();
  matrix identity();
  matrix translatev(const Vec3&);
  matrix scalev(const Vec3&);
  matrix rotatev(const Vec3&, double angle);

  matrix frustumv(double, double, double, double, double, double);
  matrix ortographicv(double, double, double, double, double, double);
  matrix perspectivev(double, double, double, double);

  matrix lookAt(Vec3, Vec3, Vec3);
} // namespace ln
