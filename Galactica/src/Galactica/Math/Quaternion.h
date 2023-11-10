#pragma once

#include  "Vec3.h"
#include  "Vec4.h"
#include "Mat3.h"

namespace Galactica
{
	/**
	 *	A quaternion is an expression of the form
			a+bi +cj +dk
		where a, b, c, d, are real numbers, and i, j, k, are symbols that can be interpreted as unit-vectors pointing along the three spatial axes.
		In practice, if one of a, b, c, d is 0, the corresponding term is omitted; if a, b, c, d are all zero, the quaternion is the zero quaternion,
		denoted 0; if one of b, c, d equals 1, the corresponding term is written simply i, j, or k. 
	 *
	 */
	template <typename T>
	class Quaternion
	{
	public:
		union
		{
			struct
			{
				T x, y, z, w;
			};

			Vec4<T> components;
		};

		Quaternion();

		Quaternion(const T x, const T y, const T z, const T w);

		Quaternion(const Vec3<T>& unitVector, const T angleInDegrees);

		Quaternion(const Quaternion<T>& quaternion);

		~Quaternion() = default;

		T Norm() const;

		T SqrNorm() const;

		Quaternion<T> GetNormalize() const;

		Quaternion<T>& Normalize();

		static Quaternion Normalize(const Quaternion& quat);

		Quaternion<T> GetRenormalize() const;

		Quaternion<T>& Renormalize();

		Quaternion<T> GetInverse() const;

		Quaternion<T>& Inverse();

		Quaternion<T> GetConjugate() const;

		static Vec3<T> TransformVector(const Quaternion<T>& quaternion, const Vec3<T>& vector);

		static T Dot(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion);

		static Quaternion<T> Lerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t);

		static Quaternion<T> Nlerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t);

		static Quaternion<T> Slerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t);

		static T LengthSquared(const Quaternion& quat);

		static T Length(const Quaternion& quat);

		Quaternion<T>& operator=(const Quaternion<T>& quaternion);
		Quaternion<T>& operator*=(const Quaternion<T>& quaternion);
		Quaternion<T>& operator*=(const T scalar);
		Quaternion<T>& operator/=(const T scalar);
		Quaternion<T>& operator+=(const Quaternion<T>& quaternion);
		Quaternion<T>& operator-=(const Quaternion<T>& quaternion);

		friend Vec3<T> operator*(const Quaternion<T>& quaternion, const Vec3<T>& vector)
		{
			return Quaternion<T>::TransformVector(quaternion, vector);
		}


	private:

		Quaternion Add(const Quaternion& quaternion);

		Quaternion Subtract(const Quaternion& quaternion);

		Quaternion Multiply(const Quaternion& quaternion);

		Quaternion Multiply(T scalar);

		Quaternion Divide(T scalar);
	};

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion);

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& quaternion, T scalar);

	template <typename T>
	Quaternion<T> operator*(T scalar, const Quaternion<T>& quaternion);

	template <typename T>
	Quaternion<T> operator/(const Quaternion<T>& quaternion, const T scalar);

	template <typename T>
	Quaternion<T> operator/(const T scalar, const Quaternion<T>& quaternion);

	template <typename T>
	Quaternion<T> operator+(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion);

	template <typename T>
	Quaternion<T> operator-(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion);

	using QuatFloat = Quaternion<float>;
}

#include "Quaternion.inl"
