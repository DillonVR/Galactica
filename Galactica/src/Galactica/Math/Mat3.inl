#pragma once

#include "Mat3.h"

namespace Galactica
{
	template <typename T>
	constexpr Mat3<T>::Mat3() noexcept
		: data{ 0 }
	{
	}

	template <typename T>
	constexpr Mat3<T>::Mat3(std::initializer_list<T> list) noexcept
	{
		std::uint8_t column = 0, row = 0;
		for (auto& element : list)
		{
			data[column][row] = element;
			column++;
			if (column == 3)
			{
				column = 0;
				row++;
			}
		}
	}

	template <typename T>
	constexpr Mat3<T>::Mat3(const Mat3<T>& matrix)noexcept
	{
		std::memcpy(data, matrix.data, sizeof(Mat3<T>));
	}

	template <typename T>
	constexpr Mat3<T>::Mat3(const Vec3<T>& firstRow,
		const Vec3<T>& secondRow,
		const Vec3<T>& thirdRow)noexcept
	{
		// Column-major ordering
		data[0][0] = firstRow.x;
		data[1][0] = firstRow.y;
		data[2][0] = firstRow.z;

		data[0][1] = secondRow.x;
		data[1][1] = secondRow.y;
		data[2][1] = secondRow.z;

		data[0][2] = thirdRow.x;
		data[1][2] = thirdRow.y;
		data[2][2] = thirdRow.z;
	}

	template <typename T>
	constexpr Mat3<T>::Mat3(T diagonal) noexcept
		: data{ 0 }
	{
		data[0][0] = diagonal;
		data[1][1] = diagonal;
		data[2][2] = diagonal;
	}

	template <typename T>
	constexpr Mat3<T>::Mat3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) noexcept
	{
		data[0][0] = m00;
		data[0][1] = m10;
		data[0][2] = m20;

		data[1][0] = m01;
		data[1][1] = m11;
		data[1][2] = m21;

		data[2][0] = m02;
		data[2][1] = m12;
		data[2][2] = m22;
	}

	template <typename T>
	Mat3<T> Mat3<T>::Inverted() const
	{
		const Vec3<T> a = reinterpret_cast<const Vec3<T>&>(data[0]);
		const Vec3<T> b = reinterpret_cast<const Vec3<T>&>(data[1]);
		const Vec3<T> c = reinterpret_cast<const Vec3<T>&>(data[2]);

		Vec3<T> bCrossC = Vec3<T>::Cross(b, c);
		Vec3<T> cCrossA = Vec3<T>::Cross(c, a);
		Vec3<T> aCrossB = Vec3<T>::Cross(a, b);

		T inverseDet = static_cast<T>(1) / Vec3<T>::Dot(aCrossB, c);

		return Mat3<T>(bCrossC.x * inverseDet, bCrossC.y * inverseDet, bCrossC.z * inverseDet,
			cCrossA.x * inverseDet, cCrossA.y * inverseDet, cCrossA.z * inverseDet,
			aCrossB.x * inverseDet, aCrossB.y * inverseDet, aCrossB.z * inverseDet);
	}

	template <typename T>
	Mat3<T>& Mat3<T>::Invert()
	{
		const Vec3<T> a = reinterpret_cast<const Vec3<T>&>(data[0]);
		const Vec3<T> b = reinterpret_cast<const Vec3<T>&>(data[1]);
		const Vec3<T> c = reinterpret_cast<const Vec3<T>&>(data[2]);

		Vec3<T> bCrossC = Vec3<T>::Cross(b, c);
		Vec3<T> cCrossa = Vec3<T>::Cross(c, a);
		Vec3<T> aCrossb = Vec3<T>::Cross(a, b);

		T inverseDet = static_cast<T>(1) / Vec3<T>::Dot(aCrossb, c);

		data[0][0] = bCrossC.x * inverseDet;
		data[0][1] = cCrossa.x * inverseDet;
		data[0][2] = aCrossb.x * inverseDet;

		data[1][0] = bCrossC.y * inverseDet;
		data[1][1] = cCrossa.y * inverseDet;
		data[1][2] = aCrossb.y * inverseDet;

		data[2][0] = bCrossC.z * inverseDet;
		data[2][1] = cCrossa.z * inverseDet;
		data[2][2] = aCrossb.z * inverseDet;

		return *this;
	}

	template <typename T>
	Mat3<T> Mat3<T>::Transposed() const
	{
		return Mat3<T>(data[0][0], data[0][1], data[0][2],
			data[1][0], data[1][1], data[1][2],
			data[2][0], data[2][1], data[2][2]);
	}

	template <typename T>
	Mat3<T>& Mat3<T>::Transpose()
	{
		Mat3<T> temp = *this;

		data[0][1] = temp.data[1][0];
		data[0][2] = temp.data[2][0];

		data[1][0] = temp.data[0][1];
		data[1][2] = temp.data[2][1];

		data[2][0] = temp.data[0][2];
		data[2][1] = temp.data[1][2];

		return *this;
	}

	template <typename T>
	T Mat3<T>::Determinant() const
	{
		return data[0][0] * (data[1][1] * data[2][2] - data[2][1] * data[1][2])
			+ data[1][0] * (data[2][1] * data[0][2] - data[0][1] * data[2][2])
			+ data[2][0] * (data[0][1] * data[1][2] - data[1][1] * data[0][2]);
	}

	template <typename T>
	Vec3<T> Mat3<T>::GetColumn(const int columnIndex) const
	{
		return col[columnIndex];
	}

	template <typename T>
	void Mat3<T>::SetColumn(const int columnIndex, const Vec3<T>& vector)
	{
		col[columnIndex] = vector;
	}

	template <typename T>
	Vec3<T> Mat3<T>::GetRow(const int rowIndex) const
	{
		return Vec3<T>(data[0][rowIndex],
			data[1][rowIndex],
			data[2][rowIndex]);
	}

	template <typename T>
	void Mat3<T>::SetRow(const int rowIndex, const Vec3<T>& vector)
	{
		data[0][rowIndex] = vector.x;
		data[1][rowIndex] = vector.y;
		data[2][rowIndex] = vector.z;
	}

	template <typename T>
	const T* Mat3<T>::GetValuePointer() const
	{
		return data[0];
	}
}
