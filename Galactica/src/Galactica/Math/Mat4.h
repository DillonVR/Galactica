#pragma once

namespace Galactica
{
	template <typename T>
	class Vec4<T>;

	template <typename T>
	class Vec3<T>;

	template <typename T>
	class Mat4
	{
	public:
		union
		{
			T data[4][4];

			Vec4<T> columns[4];
		};

		constexpr Mat4() noexcept;

		constexpr Mat4(float value) noexcept;

		constexpr  Mat4(const T m00, const T m01, const T m02, const T m03,
			const T m10, const T m11, const T m12, const T m13,
			const T m20, const T m21, const T m22, const T m23,
			const T m30, const T m31, const T m32, const T m33) noexcept;

		constexpr Mat4(std::initializer_list<T> list) noexcept;

		constexpr Mat4(const Vec4<T>& firstRow,
			const Vec4<T>& secondRow,
			const Vec4<T>& thirdRow,
			const Vec4<T>& fourthRow) noexcept;

		Vec4<T>& operator[](unsigned int i);

		const Vec4<T>& operator[](unsigned int i) const;

		bool operator==(const Mat4& leftMatrix, const Mat4& rightMatrix);

		bool operator!=(const Mat4& leftMatrix, const Mat4& rightMatrix);

		Mat4 operator+(const Mat4& leftMatrix, const Mat4& rightMatrix);

		Mat4 operator*(const Mat4& matrix, float value);

		Vec4<T> operator*(const Mat4& matrix, const Vec4<T>& vector);

		Mat4 operator*(const Mat4& leftMatrix, const Mat4& rightMatrix);

		Vec3<T> operator*(const Mat4& matrix, const Vec3<T>& vector);

		Vec3<T> TransformVector(const Mat4& matrix, const Vec3<T>& vector);

		Vec3<T> TransformPoint(const Mat4& matrix, const Vec3<T>& point);

		Vec3<T> TransformPoint(const Mat4& matrix, const Vec3<T>& point, float& w);

		Mat4<T>& Transpose();

		float Determinant() const;

		Mat4 Adjugate(const Mat4& matrix);

		Mat4<T>Invert(Mat4& matrix);

		Mat4<T>& Inverse();
	};

}
#include "Mat4.inl"
