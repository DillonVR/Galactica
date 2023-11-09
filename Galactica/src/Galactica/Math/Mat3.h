#pragma once

#include <initializer_list>

#include "Vec3.h"

namespace Galactica
{
	template <typename T>
	class Mat3
	{
	public:
		union
		{
			T data[3][3];

			std::array<Vec3<T>, 3> col;
		};

		// Constructors
		constexpr Mat3() noexcept;

		constexpr Mat3(std::initializer_list<T> list) noexcept;

		constexpr Mat3(const Vec3<T>& firstRow,
			const Vec3<T>& secondRow,
			const Vec3<T>& thirdRow)noexcept;

		constexpr Mat3(T value) noexcept;

		constexpr Mat3(const Mat3<T>& matrix) noexcept;

		constexpr Mat3(
			T x0, T y0, T z0,
			T x1, T y1, T z1,
			T x2, T y2, T z2) noexcept;

		~Mat3() = default;

		Mat3<T> Inverted() const;

		Mat3<T>& Invert();

		Mat3<T> Transposed() const;

		Mat3<T>& Transpose();

		T Determinant() const;

		Vec3<T> GetColumn(const int columnIndex) const;

		void SetColumn(const int columnIndex, const Vec3<T>& vector);

		Vec3<T> GetRow(const int rowIndex) const;

		void SetRow(const int rowIndex, const Vec3<T>& vector);

		const T* GetValuePointer() const;

		std::string ToString() const
		{
			std::string matrixString =
				"( " + std::to_string(data[0][0]) + ", " + std::to_string(data[0][1]) + ", " + std::to_string(data[0][2]) + "\n" +
				"  " + std::to_string(data[1][0]) + ", " + std::to_string(data[1][1]) + ", " + std::to_string(data[1][2]) + "\n" +
				"  " + std::to_string(data[2][0]) + ", " + std::to_string(data[2][1]) + ", " + std::to_string(data[2][2]) + " )\n";

			return matrixString;
		}
	};

}

#include "Mat3.inl"