#pragma once
#include "vector.hpp"
#include <array>
#include <ostream>
namespace ln {
	struct matrix
	{

		matrix();
		explicit matrix(std::array<double, 16>&&);
		explicit matrix(const double&);
		~matrix() = default;

		//operations on vector and matrices
		vector mulPosition(const vector&) const;
		vector mulPositionW(const vector&) const;
		double determinant() const;
		matrix inverse() const;
		matrix transpose() const;
		matrix operator*(const matrix& rhs) const;

		matrix translate(const vector&) const;
		matrix scale(const vector&) const;
		matrix rotate(const vector&, double) const;
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
	matrix translatev(const vector&);
	matrix scalev(const vector&);
	matrix rotatev(const vector&, double angle);

	matrix frustumv(double, double, double, double, double, double);
	matrix ortographicv(double, double, double, double, double, double);
	matrix perspectivev(double, double, double, double);

	matrix lookAt(vector, vector, vector);
} // namespace ln
