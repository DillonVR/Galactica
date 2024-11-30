#pragma once

#include "Quaternion.h"
#include "Constants.h"

namespace Galactica
{
	template <typename T>
	Quaternion<T>::Quaternion()
		: x(0), y(0), z(0), w(1)
	{
	}

	template <typename T>
	Quaternion<T>::Quaternion(const T x, const T y, const T z, const T w)
		: x(x), y(y), z(z), w(w)
	{
	}

	template <typename T>
	Quaternion<T>::Quaternion(const Vec3<T>& unitVector, const T angleInDegrees)
	{
		T halfAngle = DegreesToRad(angleInDegrees) / static_cast<T>(2);
		T cosHalfAngle = std::cos(halfAngle);
		T sinHalfAngle = std::sin(halfAngle);

		x = unitVector.x * sinHalfAngle;
		y = unitVector.y * sinHalfAngle;
		z = unitVector.z * sinHalfAngle;
		w = cosHalfAngle;
	}

	template <typename T>
	Quaternion<T>::Quaternion(const Quaternion<T>& quaternion)
		: x(quaternion.x), y(quaternion.y), z(quaternion.z), w(quaternion.w)
	{
	}

	template<typename T>
	inline Quaternion<T>::Quaternion(const glm::quat& q)
		: x(q.x), y(q.y), z(q.z), w(q.w)
	{
	}

	template <typename T>
	T Quaternion<T>::Norm() const
	{
		return std::sqrt(SqrNorm());
	}

	template <typename T>
	T Quaternion<T>::SqrNorm() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::GetNormalize() const
	{
		T norm = Norm();
		return Quaternion<T>(x / norm, y / norm, z / norm, w / norm);
	}

	template<typename T>
	Quaternion<T>& Quaternion<T>::Normalize()
	{
		T norm = Norm();
		this->x /= norm;
		this->y /= norm;
		this->z /= norm;
		this->w /= norm;
		return *this;
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::Normalize(const Quaternion& quat)
	{
		Quaternion normalizedQuat(quat);

		T length = Length(normalizedQuat);

		if (length <= static_cast<T>(0))
		{
			return Quaternion();
		}

		float inverseLength = static_cast<T>(1) / Length(normalizedQuat);

		normalizedQuat.x *= inverseLength;
		normalizedQuat.y *= inverseLength;
		normalizedQuat.z *= inverseLength;
		normalizedQuat.w *= inverseLength;
		return normalizedQuat;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::GetRenormalize() const
	{
		T inverseNorm = FastInverseSquareRootAroundOne(SqrNorm());
		return Quaternion<T>(x * inverseNorm, y * inverseNorm, z * inverseNorm, w * inverseNorm);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::Renormalize()
	{
		T inverseNorm = FastInverseSquareRootAroundOne(SqrNorm());
		*this *= inverseNorm;
		return *this;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::GetInverse() const
	{
		return GetConjugate();
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::Inverse()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::GetConjugate() const
	{
		return Quaternion<T>(-x, -y, -z, w);
	}

	template <typename T>
	Vec3<T> Quaternion<T>::TransformVector(const Quaternion<T>& quaternion, const Vec3<T>& vector)
	{
		Vec3<T> vectorPart(quaternion.x, quaternion.y, quaternion.z);

		float scalar = quaternion.w;

		return static_cast<T>(2) * Vec3<T>::Dot(vectorPart, vector) * vectorPart
			+ (scalar * scalar - Vec3<T>::Dot(vectorPart, vectorPart)) * vector
			+ static_cast<T>(2) * scalar * Vec3<T>::Cross(vectorPart, vector);
	}

	template <typename T>
	T Quaternion<T>::Dot(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion)
	{
		return leftQuaternion.x * rightQuaternion.x +
			leftQuaternion.y * rightQuaternion.y +
			leftQuaternion.z * rightQuaternion.z +
			leftQuaternion.w * rightQuaternion.w;
	}


	template <typename T>
	Quaternion<T> Quaternion<T>::Lerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t)
	{
		return leftQuaternion * (static_cast<T>(1) - t) + rightQuaternion * t;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Nlerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion, const T t)
	{
		return Lerp(leftQuaternion, rightQuaternion, t).GetNormalize();
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Slerp(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion,T t)
	{
		T dot = Quaternion<T>::Dot(leftQuaternion, rightQuaternion);

		Quaternion rightCopy = rightQuaternion;

		if (dot < static_cast<T>(0))
		{
			rightCopy = static_cast<T>(-1) * rightCopy;
			dot = -dot;
		}

		if (dot > static_cast<T>(1) - EPSILON)
		{
			return Nlerp(leftQuaternion, rightQuaternion, t);
		}

		T clampT = std::clamp(t, static_cast<T>(0), static_cast<T>(1));
		T theta = std::acos(dot); 
		
		return (std::sin((static_cast<T>(1) - clampT) * theta) * leftQuaternion + std::sin(clampT * theta) * rightCopy) / std::sin(theta);
	}

	template <typename T>
	T Quaternion<T>::LengthSquared(const Quaternion& quat)
	{
		return (
			quat.x * quat.x +
			quat.y * quat.y +
			quat.z * quat.z +
			quat.w * quat.w);
	}

	template <typename T>
	T Quaternion<T>::Length(const Quaternion& quat)
	{
		T lengthSquared = LengthSquared(quat);

		if (lengthSquared <= static_cast<T>(EPSILON))
			return static_cast<T>(0);

		return std::sqrt(lengthSquared);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator=(const Quaternion<T>& quaternion)
	{
		x = quaternion.x;
		y = quaternion.y;
		z = quaternion.z;
		w = quaternion.w;
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<T>& quaternion)
	{
		Quaternion<T> temp = *this;

		this->x = temp.w * quaternion.x + temp.x * quaternion.w + temp.y * quaternion.z - temp.z * quaternion.y;
		this->y = temp.w * quaternion.y + temp.y * quaternion.w + temp.z * quaternion.x - temp.x * quaternion.z;
		this->z = temp.w * quaternion.z + temp.z * quaternion.w + temp.x * quaternion.y - temp.y * quaternion.x;
		this->w = temp.w * quaternion.w - temp.x * quaternion.x - temp.y * quaternion.y - temp.z * quaternion.z;

		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator+=(const Quaternion<T>& quaternion)
	{
		x += quaternion.x;
		y += quaternion.y;
		z += quaternion.z;
		w += quaternion.w;
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator-=(const Quaternion<T>& quaternion)
	{
		x -= quaternion.x;
		y -= quaternion.y;
		z -= quaternion.z;
		w -= quaternion.w;
		return *this;
	}

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion)
	{
		Quaternion<T> leftQuaterionCopy = leftQuaternion;
		return leftQuaterionCopy *= rightQuaternion;
	}

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& quaterion, T scalar)
	{
		Quaternion quaterionCopy = quaterion;
		return quaterionCopy *= scalar;
	}

	template<typename T>
	Quaternion<T> operator*(T scalar, const Quaternion<T>& quaternion)
	{
		Quaternion quaterionCopy = quaternion;
		return quaterionCopy *= scalar;
	}

	template <typename T>
	Quaternion<T> operator/(const Quaternion<T>& quaternion, const T scalar)
	{
		Quaternion<T> quaterionCopy = quaternion;
		return quaterionCopy /= scalar;
	}

	template<typename T>
	Quaternion<T> operator/(const T scalar, const Quaternion<T>& quaternion)
	{
		Quaternion<T> quaterionCopy = quaternion;
		return quaterionCopy /= scalar;
	}

	template <typename T>
	Quaternion<T> operator+(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion)
	{
		Quaternion<T> leftQuaterionCopy = leftQuaternion;
		return leftQuaterionCopy += rightQuaternion;
	}

	template <typename T>
	Quaternion<T> operator-(const Quaternion<T>& leftQuaternion, const Quaternion<T>& rightQuaternion)
	{
		Quaternion<T> leftQuaterionCopy = leftQuaternion;
		return leftQuaterionCopy -= rightQuaternion;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Add(const Quaternion& quaternion)
	{
		return (*this) + quaternion;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Subtract(const Quaternion& quaternion)
	{
		return (*this) - quaternion;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Multiply(const Quaternion& quaternion)
	{
		return (*this) * quaternion;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Multiply(T scalar)
	{
		return (*this) * scalar;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Divide(T scalar)
	{
		return (*this) / scalar;
	}
}
