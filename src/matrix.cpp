#include "matrix.hpp"
#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <random>
namespace ln {

  auto dice()
  {
    static std::uniform_real_distribution<double> distr{0, 1};
    static std::random_device device;
    static std::mt19937 engine{device()};
    return distr(engine);
  }
  matrix::matrix(){};

  matrix::matrix(std::array<double, 16>&& m)
      : m_data{m}
  { }

  // initialize all elements to a given value
  matrix::matrix(const double& val)
  {
    std::fill(m_data.begin(), m_data.end(), val);
  }

  /**  The schema of 4 x 4 matrix maps onto of 16 array in memory.  
  *      1   2  3  4 (n)
  * 1    0   1  2  3 
  * 2    4   5  6  7
  * 3    8   9  10 11
  * 4    12  13 14 15
  * (m)
  */
  double matrix::getElement(int m, int n) const
  {
    return m_data[(4 * (m - 1)) + (n - 1)];
  }

  std::string matrix::print() const
  {
    std::string s;
    for(size_t i = 0; i < 4; i++) {
      for(size_t j = 0; j < 4; j++) {
        s += std::format(" {:.2f} ", m_data[j + 4 * i]);
      }
      s += "\n";
    }

    return s;
  };

  std::ostream& operator<<(std::ostream& stream, const matrix& matrix)
  {
    return std::cout << matrix.print();
  };

  // initialize all elements to a given value
  matrix random_matrix()
  {
    std::array<double, 16> m;
    std::generate(m.begin(), m.end(), dice);
    return matrix(std::move(m));
  }

  matrix identity()
  {
    auto id = [idx = -1]() mutable {
      ++idx;
      return idx % 5 == 0 ? 1 : 0;
    };
    std::array<double, 16> v;
    std::generate(v.begin(), v.end(), id);
    return matrix(std::move(v));
  }
  matrix translatev(const vector& vec)
  {
    matrix id = identity();
    id.m_data[3] = vec.v_data[0];
    id.m_data[7] = vec.v_data[1];
    id.m_data[11] = vec.v_data[2];
    return id;
  };

  matrix scalev(const vector& vec)
  {
    matrix id = identity();
    id.m_data[0] = vec.v_data[0];
    id.m_data[5] = vec.v_data[1];
    id.m_data[10] = vec.v_data[2];
    return id;
  };

  //https://stackoverflow.com/questions/42749032/concatenating-a-sequence-of-stdarrays
  template <class T, size_t Na, size_t Nb>
  constexpr auto array_cat(const std::array<T, Na>& a, const std::array<T, Nb>& b, auto&&... rest)
  {
    if constexpr(sizeof...(rest) == 0) {
      return std::apply(
          [](auto&&... elements) {
            return std::array{std::forward<decltype(elements)>(elements)...};
          },
          std::tuple_cat(a, b));
    }
    else {
      return array_cat(a, array_cat(b, std::forward<decltype(rest)>(rest)...));
    }
  }

  matrix rotatev(const vector& vec, double angle)
  {
    auto v = vec.normalize().v_data;
    auto s = std::sin(angle);
    auto c = std::cos(angle);
    auto m = 1 - c;

    //first row
    std::array<double, 4> m1{
        m * v[0] * v[0] + c, m * v[0] * v[1] + v[2] * s, m * v[2] * v[0] - v[1] * s, 0};
    //second row
    std::array<double, 4> m2{
        m * v[0] * v[1] - v[2] * s, m * v[1] * v[1] + c, m * v[1] * v[2] + v[0] * s, 0};
    //third row
    std::array<double, 4> m3{
        m * v[2] * v[0] + v[1] * s, m * v[1] * v[2] - v[0] * s, m * v[2] * v[2] + c, 0};
    //fourth row
    std::array<double, 4> m4{0, 0, 0, 1};
    std::array<double, 16> rot_matrix = array_cat(m1, m2, m3, m4);
    return matrix{std::move(rot_matrix)};
  };

  matrix frustumv(double l, double r, double b, double t, double n, double f)
  {
    auto t1 = 2 * n;
    auto t2 = r - l;
    auto t3 = t - b;
    auto t4 = f - n;
    std::array<double, 16> pre{t1 / t2,
                               0,
                               (r + l) / t2,
                               0,
                               0,
                               t1 / t3,
                               (t + b) / t3,
                               0,
                               0,
                               0,
                               (-f - n) / t4,
                               (-t1 * f) / t4,
                               0,
                               0,
                               -1,
                               0};
    return matrix{std::move(pre)};
  }

  matrix ortographicv(double l, double r, double b, double t, double n, double f)
  {
    std::array<double, 16> pre{2 / (r - l),
                               0,
                               0,
                               -(r + l) / (r - l),
                               0,
                               2 / (t - b),
                               0,
                               -(t + b) / (t - b),
                               0,
                               0,
                               -2 / (f - n),
                               (-f + n) / (f - n),
                               0,
                               0,
                               0,
                               1};
    return matrix(std::move(pre));
  }
  matrix perspectivev(double fovy, double aspect, double near, double far)
  {
    auto ymax = near * std::tan(fovy * std::numbers::pi / 360);
    auto xmax = ymax * aspect;
    auto f = frustumv(-xmax, xmax, -ymax, ymax, near, far);

    return f;
  }
  vector matrix::mulPosition(const vector& b) const
  {
    auto v = b.v_data;
    auto x = m_data[0] * v[0] + m_data[1] * v[1] + m_data[2] * v[2] + m_data[3];
    auto y = m_data[4] * v[0] + m_data[5] * v[1] + m_data[6] * v[2] + m_data[7];
    auto z = m_data[8] * v[0] + m_data[9] * v[1] + m_data[10] * v[2] + m_data[11];
    return vector{x, y, z};
  }
  vector matrix::mulPositionW(const vector& b) const
  {
    auto v = b.v_data;
    auto x = m_data[0] * v[0] + m_data[1] * v[1] + m_data[2] * v[2] + m_data[3];
    auto y = m_data[4] * v[0] + m_data[5] * v[1] + m_data[6] * v[2] + m_data[7];
    auto z = m_data[8] * v[0] + m_data[9] * v[1] + m_data[10] * v[2] + m_data[11];
    auto w = m_data[12] * v[0] + m_data[13] * v[1] + m_data[14] * v[2] + m_data[15];
    return vector{x, y, z} / w;
  }
  matrix lookAt(vector eye, vector center, vector ups)
  {
    auto up = ups.normalize();
    auto f = (center - eye).normalize();
    auto s = f.cross(up).normalize();
    auto u = s.cross(f).normalize();
    std::array<double, 16> pre{s.v_data[0],
                               u.v_data[0],
                               -f.v_data[0],
                               eye.v_data[0],
                               s.v_data[1],
                               u.v_data[1],
                               -f.v_data[1],
                               eye.v_data[1],
                               s.v_data[2],
                               u.v_data[2],
                               -f.v_data[2],
                               eye.v_data[2],
                               0,
                               0,
                               0,
                               1};
    auto m = matrix{std::move(pre)};

    return m.inverse();
  };

  matrix matrix::transpose() const
  {
    std::array<double, 16> pre{m_data[0],
                               m_data[4],
                               m_data[8],
                               m_data[12],
                               m_data[1],
                               m_data[5],
                               m_data[9],
                               m_data[13],
                               m_data[2],
                               m_data[6],
                               m_data[10],
                               m_data[14],
                               m_data[3],
                               m_data[7],
                               m_data[11],
                               m_data[15]};
    return matrix{std::move(pre)};
  }

  matrix matrix::operator*(const matrix& rhs) const
  {
    std::array<double, 16> mul;
    auto a = m_data;
    auto b = rhs.m_data;
    mul[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
    mul[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
    mul[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
    mul[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
    mul[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
    mul[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
    mul[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
    mul[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
    mul[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
    mul[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
    mul[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
    mul[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
    mul[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];
    mul[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];
    mul[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];
    mul[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
    return matrix{std::move(mul)};
  };

  matrix matrix::translate(const vector& v) const
  {
    auto t = translatev(v);
    return t * *(this);
  }

  matrix matrix::scale(const vector& v) const
  {
    auto s = scalev(v);
    return s * *(this);
  }

  matrix matrix::rotate(const vector& v, double a) const
  {
    auto r = rotatev(v, a);
    return r * *(this);
  }

  matrix matrix::frustum(double l, double r, double b, double t, double n, double f) const
  {

    return frustumv(l, r, b, t, n, f) * *(this);
  }

  matrix matrix::ortographic(double l, double r, double b, double t, double n, double f) const
  {

    return ortographicv(l, r, b, t, n, f) * *(this);
  }

  matrix matrix::perspective(double fovy, double aspect, double near, double far) const
  {

    return perspectivev(fovy, aspect, near, far) * *(this);
  }

  matrix matrix::inverse() const
  {

    std::array<double, 16> inv;
    double det;
    auto m = m_data;

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
             m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] -
             m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
             m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] -
              m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] -
             m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
             m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
             m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
              m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] +
             m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
             m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
              m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] -
              m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] -
             m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] -
              m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if(det == 0)
      throw std::runtime_error("non invertible");

    det = 1.0 / det;

    std::array<double, 16> inv_out;
    for(auto i = 0; i < 16; i++)
      inv_out[i] = inv[i] * det;

    return matrix(std::move(inv_out));
  };
} // namespace ln
