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
		Quaternion newQuat(quat);

		T length = Length(newQuat);

		if (length <= static_cast<T>(0))
		{
			return Quaternion();
		}

		float inverseLength = static_cast<T>(1) / Length(newQuat);
		newQuat.x *= inverseLength;
		newQuat.y *= inverseLength;
		newQuat.z *= inverseLength;
		newQuat.w *= inverseLength;
		return newQuat;
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
	Mat3<T> Quaternion<T>::GetRotationMatrix() const
	{
		Mat3<T> matrix;

		T xSquared = x * x;
		T ySquared = y * y;
		T zSquared = z * z;
		T xy = x * y;
		T xz = x * z;
		T yz = y * z;
		T wx = w * x;
		T wy = w * y;
		T wz = w * z;

		matrix(0, 0) = static_cast<T>(1) - static_cast<T>(2) * (ySquared + zSquared);
		matrix(1, 0) = static_cast<T>(2) * (xy + wz);
		matrix(2, 0) = static_cast<T>(2) * (xz - wy);

		matrix(0, 1) = static_cast<T>(2) * (xy - wz);
		matrix(1, 1) = static_cast<T>(1) - static_cast<T>(2) * (xSquared + zSquared);
		matrix(2, 1) = static_cast<T>(2) * (yz + wx);

		matrix(0, 2) = static_cast<T>(2) * (xz + wy);
		matrix(1, 2) = static_cast<T>(2) * (yz - wx);
		matrix(2, 2) = static_cast<T>(1) - static_cast<T>(2) * (xSquared + ySquared);

		return matrix;
	}

	template <typename T>
	void Quaternion<T>::SetOrthogonalRotationMatrix(const Mat3<T>& matrix)
	{
		T m00 = matrix(0, 0);
		T m11 = matrix(1, 1);
		T m22 = matrix(2, 2);
		T diagonalSum = m00 + m11 + m22;

		if (diagonalSum > static_cast<T>(0))
		{
			T fraction = static_cast<T>(0.5) / std::sqrt(diagonalSum + static_cast<T>(1));

			x = (matrix(2, 1) - matrix(1, 2)) * fraction;
			y = (matrix(0, 2) - matrix(2, 0)) * fraction;
			z = (matrix(1, 0) - matrix(0, 1)) * fraction;
			w = static_cast<T>(0.25) / fraction;
		}
		else if (m00 > m11 && m00 > m22)
		{
			T fraction = std::sqrt(m00 - m11 - m22 + static_cast<T>(1)) * static_cast<T>(2);

			x = static_cast<T>(0.25) * fraction;
			y = (matrix(0, 1) + matrix(1, 0)) / fraction;
			z = (matrix(0, 2) + matrix(2, 0)) / fraction;
			w = (matrix(2, 1) - matrix(1, 2)) / fraction;
		}
		else if (m11 > m22)
		{
			T fraction = std::sqrt(m11 - m00 - m22 + static_cast<T>(1)) * static_cast<T>(2);

			x = (matrix(0, 1) + matrix(1, 0)) / fraction;
			y = static_cast<T>(0.25) * fraction;
			z = (matrix(1, 2) + matrix(2, 1)) / fraction;
			w = (matrix(0, 2) - matrix(2, 0)) / fraction;
		}
		else
		{
			T fraction = std::sqrt(m22 - m00 - m11 + static_cast<T>(1)) * static_cast<T>(2);

			x = (matrix(0, 2) + matrix(2, 0)) / fraction;
			y = (matrix(1, 2) + matrix(2, 1)) / fraction;
			z = static_cast<T>(0.25) * fraction;
			w = (matrix(1, 0) - matrix(0, 1)) / fraction;
		}
	}

	template <typename T>
	void Quaternion<T>::SetNonOrthogonalRotationMatrix(const Mat3<T>& matrix)
	{
		T m00 = matrix(0, 0);
		T m11 = matrix(1, 1);
		T m22 = matrix(2, 2);

		float absQ2 = std::pow(matrix.Determinant(), static_cast<T>(1) / static_cast<T>(3));

		w = std::sqrt(std::max<T>(static_cast<T>(0), absQ2 + m00 + m11 + m22)) / static_cast<T>(2);
		x = std::sqrt(std::max<T>(static_cast<T>(0), absQ2 + m00 - m11 - m22)) / static_cast<T>(2);
		y = std::sqrt(std::max<T>(static_cast<T>(0), absQ2 - m00 + m11 - m22)) / static_cast<T>(2);
		z = std::sqrt(std::max<T>(static_cast<T>(0), absQ2 - m00 - m11 + m22)) / static_cast<T>(2);

		x = std::copysign(x, matrix(2, 1) - matrix(1, 2));
		y = std::copysign(y, matrix(0, 2) - matrix(2, 0));
		z = std::copysign(z, matrix(1, 0) - matrix(0, 1));
	}

	template <typename T>
	Vec3<T> Quaternion<T>::GetEulerAngles() const
	{
		Vec3<T> angles;

		T singularityTest = x * y + z * w;

		T one = static_cast<T>(1);
		T two = static_cast<T>(2);

		if (singularityTest > NORTH_POLE_SINGULARITY_VALUE)
		{
			angles.y = two * std::atan2(x, w);
			angles.z = static_cast<T>(PI_F) / two;
		}
		else if (singularityTest < SOUTH_POLE_SINGULARITY_VALUE)
		{
			angles.y = -two * std::atan2(x, w);
			angles.z = -static_cast<T>(PI_F) / two;
		}
		else
		{
			T xSquared = x * x;
			T ySquared = y * y;
			T zSquared = z * z;

			angles.y = std::atan2(two * y * w - two * x * z, one - two * ySquared - two * zSquared);
			angles.z = std::asin(two * singularityTest);
			angles.x = std::atan2(two * x * w - two * y * z, one - two * xSquared - two * zSquared);
		}

		angles.x = RadToDegrees(angles.x);
		angles.y = RadToDegrees(angles.y);
		angles.z = RadToDegrees(angles.z);

		return angles;
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
	Quaternion<T> Quaternion<T>::MakeRotationX(const T angleInDegrees)
	{
		T halfAngle = DegreesToRad(angleInDegrees) / static_cast<T>(2);
		return Quaternion<T>(std::sin(halfAngle), static_cast<T>(0), static_cast<T>(0), std::cos(halfAngle));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationY(const T angleInDegrees)
	{
		T halfAngle = DegreesToRad(angleInDegrees) / static_cast<T>(2);
		return Quaternion<T>(static_cast<T>(0), std::sin(halfAngle), static_cast<T>(0), std::cos(halfAngle));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationZ(const T angleInDegrees)
	{
		float halfAngle = DegreesToRad(angleInDegrees) / static_cast<T>(2);
		return Quaternion<T>(static_cast<T>(0), static_cast<T>(0), std::sin(halfAngle), std::cos(halfAngle));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationAxisAngle(const Vec3<T>& unitVector, const T angleInDegrees)
	{
		return Quaternion<T>(unitVector, angleInDegrees);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeRotationFromEulers(Vec3<T> angles)
	{
		T two = static_cast<T>(2);

		T xHalfAngle = DegreesToRad(angles.x / two);
		T yHalfAngle = DegreesToRad(angles.y / two);
		T zHalfAngle = DegreesToRad(angles.z / two);

		T xCos = std::cos(xHalfAngle);
		T yCos = std::cos(yHalfAngle);
		T zCos = std::cos(zHalfAngle);

		T xSin = std::sin(xHalfAngle);
		T ySin = std::sin(yHalfAngle);
		T zSin = std::sin(zHalfAngle);

		T zCosXCos = zCos * xCos;
		T zSinXSin = zSin * xSin;
		T zSinXCos = zSin * xCos;
		T zCosXSin = zCos * xSin;

		T w = zCosXCos * yCos + zSinXSin * ySin;
		T x = zCosXSin * yCos - zSinXCos * ySin;
		T y = zCosXCos * ySin + zSinXSin * yCos;
		T z = zSinXCos * yCos - zCosXSin * ySin;

		return Quaternion(x, y, z, w);
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
