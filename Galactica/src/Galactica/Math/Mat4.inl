#pragma once

#include "Mat4.h"

namespace Galactica
{
	template <typename T>
	constexpr Mat4<T>::Mat4() noexcept
		:	data{0}
	{
	}

	template <typename T>
	constexpr Mat4<T>::Mat4(float diagonal) noexcept
		: data{ 0 }
	{
		data[0][0] = diagonal;
		data[1][1] = diagonal;
		data[2][2] = diagonal;
		data[3][3] = diagonal;
	}

	template <typename T>
	constexpr  Mat4<T>::Mat4(const T m00, const T m01, const T m02, const T m03,
		const T m10, const T m11, const T m12, const T m13,
		const T m20, const T m21, const T m22, const T m23,
		const T m30, const T m31, const T m32, const T m33) noexcept
		: data{ 0 }
	{
		// Column-major ordering
		data[0][0] = m00;
		data[0][1] = m10;
		data[0][2] = m20;
		data[0][3] = m30;

		data[1][0] = m01;
		data[1][1] = m11;
		data[1][2] = m21;
		data[1][3] = m31;

		data[2][0] = m02;
		data[2][1] = m12;
		data[2][2] = m22;
		data[2][3] = m32;

		data[3][0] = m03;
		data[3][1] = m13;
		data[3][2] = m23;
		data[3][3] = m33;
	}

	template <typename T>
	constexpr Mat4<T>::Mat4(std::initializer_list<T> list) noexcept
		: data{ 0 }
	{
		std::uint8_t column = 0, row = 0;
		for (auto& element : list)
		{
			this->data[column][row] = element;
			column++;
			if (column == 4)
			{
				column = 0;
				row++;
			}
		}
	}

	template <typename T>
	Mat4<T>::Mat4(const Vec4<T>& firstRow,
		const Vec4<T>& secondRow,
		const Vec4<T>& thirdRow,
		const Vec4<T>& fourthRow) noexcept
		: data{ 0 }
	{

		data[0][0] = firstRow.x;
		data[1][0] = firstRow.y;
		data[2][0] = firstRow.z;
		data[3][0] = firstRow.w;

		data[0][1] = secondRow.x;
		data[1][1] = secondRow.y;
		data[2][1] = secondRow.z;
		data[3][1] = secondRow.w;

		data[0][2] = thirdRow.x;
		data[1][2] = thirdRow.y;
		data[2][2] = thirdRow.z;
		data[3][2] = thirdRow.w;

		data[0][3] = fourthRow.x;
		data[1][3] = fourthRow.y;
		data[2][3] = fourthRow.z;
		data[3][3] = fourthRow.w;
	}

	template <typename T>
	Vec4<T>& Mat4<T>::operator[](unsigned i)
	{
		return this->columns[i];
	}

	template <typename T>
	const Vec4<T>& Mat4<T>::operator[](unsigned i) const
	{
		return this->columns[i];
	}

	template <typename T>
	bool Mat4<T>::operator==(const Mat4& leftMatrix, const Mat4& rightMatrix)
	{
		for (int i = 0; i < 16; ++i) {
			if (std::fabsf(leftMatrix.data[i] - rightMatrix.data[i]) > EPSILON) {
				return false;
			}
		}
		return true;
	}

	template <typename T>
	bool Mat4<T>::operator!=(const Mat4& leftMatrix, const Mat4& rightMatrix)
	{
		return !(leftMatrix == rightMatrix);
	}

	template <typename T>
	Mat4<T> Mat4<T>::operator+(const Mat4& leftMatrix, const Mat4& rightMatrix)
	{
		Mat4 left_matrix(leftMatrix);
		return left_matrix += rightMatrix;
	}

	template <typename T>
	Mat4<T> Mat4<T>::operator*(const Mat4& matrix, float value)
	{
		Mat4 matrix1(matrix);
		return matrix1 *= value;
	}

	template <typename T>
	Vec4<T> Mat4<T>::operator*(const Mat4& matrix, const Vec4<T>& vector)
	{
		Mat4 matrix1(matrix);
		return matrix1 *= vector;
	}

	template <typename T>
	Mat4<T> Mat4<T>::operator*(const Mat4& leftMatrix, const Mat4& rightMatrix)
	{
		Mat4 left_matrix(leftMatrix);
		return left_matrix *= rightMatrix;
	}

	template <typename T>
	Vec3<T> Mat4<T>::operator*(const Mat4& matrix, const Vec3<T>& vector)
	{
		Mat4 matrix1(matrix);
		return matrix1 *= vector;
	}

	template <typename T>
	Vec3<T> Mat4<T>::TransformVector(const Mat4& matrix, const Vec3<T>& vector)
	{
		return Vec3<T>(
			matrix[0] * vector,
			matrix[1] * vector,
			matrix[2] * vector
		);
	}

	template <typename T>
	Vec3<T> Mat4<T>::TransformPoint(const Mat4& matrix, const Vec3<T>& point)
	{
		columns.reserve(4);
		columns.emplace_back(point, 1);
		columns.emplace_back(point, 1);
		columns.emplace_back(point, 1);
		columns.emplace_back(point, 1);

		return Vec3<T>(
			matrix[0] * point,
			matrix[1] * point,
			matrix[2] * point
		);
	}

	template <typename T>
	Vec3<T> Mat4<T>::TransformPoint(const Mat4& matrix, const Vec3<T>& point, float& w)
	{
		columns.reserve(4);
		columns.emplace_back(point, w);
		columns.emplace_back(point, w);
		columns.emplace_back(point, w);
		columns.emplace_back(point, w);

		return Vec3<T>(
			matrix[0] * point,
			matrix[1] * point,
			matrix[2] * point
		);
	}

	template <typename T>
	Mat4<T>& Mat4<T>::Transpose()
	{
		Mat4<T> temp = *this;

		data[0][1] = temp.data[1][0];
		data[0][2] = temp.data[2][0];
		data[0][3] = temp.data[3][0];

		data[1][0] = temp.data[0][1];
		data[1][2] = temp.data[2][1];
		data[1][3] = temp.data[3][1];

		data[2][0] = temp.data[0][2];
		data[2][1] = temp.data[1][2];
		data[2][3] = temp.data[3][2];

		data[3][0] = temp.data[0][3];
		data[3][1] = temp.data[1][3];
		data[3][2] = temp.data[2][3];

		return *this;
	}


	template <typename T>
	float Mat4<T>::Determinant() const
	{
		const Vec3<T> a = reinterpret_cast<const Vec3<T>&>(data[0]);
		const Vec3<T> b = reinterpret_cast<const Vec3<T>&>(data[1]);
		const Vec3<T> c = reinterpret_cast<const Vec3<T>&>(data[2]);
		const Vec3<T> d = reinterpret_cast<const Vec3<T>&>(data[3]);

		Vec3<T> s = Vec3<T>::Cross(a, b);
		Vec3<T> t = Vec3<T>::Cross(c, d);
		Vec3<T> u = a * data[1][3] - b * data[0][3];
		Vec3<T> v = c * data[3][3] - d * data[2][3];

		return Vec3<T>::Dot(s, v) - Vec3<T>::Dot(t, u);
	}

	//TODO:
	template <typename T>
	Mat4<T> Mat4<T>::Adjugate(const Mat4& matrix)
	{
		return {};
	}

	template <typename T>
	Mat4<T> Mat4<T>::Invert(Mat4& matrix)
	{
		const Vec3<T> a = reinterpret_cast<const Vec3<T>&>(data[0]);
		const Vec3<T> b = reinterpret_cast<const Vec3<T>&>(data[1]);
		const Vec3<T> c = reinterpret_cast<const Vec3<T>&>(data[2]);
		const Vec3<T> d = reinterpret_cast<const Vec3<T>&>(data[3]);

		T x = data[0][3];
		T y = data[1][3];
		T z = data[2][3];
		T w = data[3][3];

		Vec3<T> s = Vec3<T>::Cross(a, b);
		Vec3<T> t = Vec3<T>::Cross(c, d);
		Vec3<T> u = a * y - b * x;
		Vec3<T> v = c * w - d * z;

		T inverseDet = static_cast<T>(1) / (Vec3<T>::Dot(s, v) + Vec3<T>::Dot(t, u));
		s *= inverseDet;
		t *= inverseDet;
		u *= inverseDet;
		v *= inverseDet;

		Vec3<T> firstRow = Vec3<T>::Cross(b, v) + t * y;
		Vec3<T> secondRow = Vec3<T>::Cross(v, a) - t * x;
		Vec3<T> thirdRow = Vec3<T>::Cross(d, u) + s * w;
		Vec3<T> fourthRow = Vec3<T>::Cross(u, c) - s * z;

		return Mat4<T>(firstRow.x, firstRow.y, firstRow.z, -Vec3<T>::Dot(b, t),
			secondRow.x, secondRow.y, secondRow.z, Vec3<T>::Dot(a, t),
			thirdRow.x, thirdRow.y, thirdRow.z, -Vec3<T>::Dot(d, s),
			fourthRow.x, fourthRow.y, fourthRow.z, Vec3<T>::Dot(c, s));
	}

	template <typename T>
	Mat4<T>& Mat4<T>::Inverse()
	{
		const Vec3<T> a = reinterpret_cast<const Vec3<T>&>(data[0]);
		const Vec3<T> b = reinterpret_cast<const Vec3<T>&>(data[1]);
		const Vec3<T> c = reinterpret_cast<const Vec3<T>&>(data[2]);
		const Vec3<T> d = reinterpret_cast<const Vec3<T>&>(data[3]);

		T x = data[0][3];
		T y = data[1][3];
		T z = data[2][3];
		T w = data[3][3];

		Vec3<T> s = Vec3<T>::Cross(a, b);
		Vec3<T> t = Vec3<T>::Cross(c, d);
		Vec3<T> u = a * y - b * x;
		Vec3<T> v = c * w - d * z;

		T inverseDet = static_cast<T>(1) / (Vec3<T>::Dot(s, v) + Vec3<T>::Dot(t, u));
		s *= inverseDet;
		t *= inverseDet;
		u *= inverseDet;
		v *= inverseDet;

		Vec3<T> firstRow = Vec3<T>::Cross(b, v) + t * y;
		Vec3<T> secondRow = Vec3<T>::Cross(v, a) - t * x;
		Vec3<T> thirdRow = Vec3<T>::Cross(d, u) + s * w;
		Vec3<T> fourthRow = Vec3<T>::Cross(u, c) - s * z;

		data[0][0] = firstRow.x;
		data[1][0] = firstRow.y;
		data[2][0] = firstRow.z;
		data[3][0] = -Vec3<T>::Dot(b, t);

		data[0][1] = secondRow.x;
		data[1][1] = secondRow.y;
		data[2][1] = secondRow.z;
		data[3][1] = Vec3<T>::Dot(a, t);

		data[0][2] = thirdRow.x;
		data[1][2] = thirdRow.y;
		data[2][2] = thirdRow.z;
		data[3][2] = -Vec3<T>::Dot(d, s);

		data[0][3] = fourthRow.x;
		data[1][3] = fourthRow.y;
		data[2][3] = fourthRow.z;
		data[3][3] = Vec3<T>::Dot(c, s);

		return *this;
	}
}
