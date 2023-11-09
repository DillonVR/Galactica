#pragma once

#include "Vec3.h"
namespace Galactica
{
	template <typename T>
	constexpr Vec3<T>::Vec3() noexcept
		: x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0))
	{
	}

	template <typename T>
	constexpr Vec3<T>::Vec3(T x, T y, T z) noexcept
		: x(x), y(y), z(z)
	{
	}

	template <typename T>
	constexpr Vec3<T>::Vec3(T value) noexcept
		: x(value), y(value), z(value)
	{
	}

	template <typename T>
	constexpr Vec3<T>::Vec3(std::initializer_list<T> data) noexcept
	{
		GL_CORE_ASSERT(data.size() == 3, "Initializer List needs 3 arguments!");
		std::copy(data.begin(), data.end(), variables.begin());
	}

	template <typename T>
	constexpr Vec3<T>::Vec3(const Vec3& vector) noexcept
		: x(vector.x), y(vector.y), z(vector.z)
	{
	}

	// Operations
	template <typename T>
	Vec3<T>& Vec3<T>::operator=(const Vec3& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator+(const Vec3& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vec3(x + vector.x, y + vector.y, z + vector.z);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator+=(const Vec3& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator-(const Vec3& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vec3(x - vector.x, y - vector.y, z - vector.z);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator-=(const Vec3& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator*(T value)
	{
		GL_CORE_ASSERT(!std::isnan(value), "Operation '/' failed, the value is NaN!");
		return Vec3(x * value, y * value, z * value);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator*=(T value)
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator/(T value)
	{
		GL_CORE_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		return Vec3(x * inverse, y * inverse, z * inverse);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator/=(T value)
	{
		GL_CORE_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		x *= inverse;
		y *= inverse;
		z *= inverse;
		return *this;
	}

	template <typename T>
	bool Vec3<T>::operator==(const Vec3& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x == vector.x && y == vector.y && z == vector.z;
	}

	template <typename T>
	bool Vec3<T>::operator!=(const Vec3& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x != vector.x || y != vector.y || z != vector.z;
	}

	template <typename T>
	const T* Vec3<T>::GetPointerToData() const
	{
		return variables.data();
	}

	// Core Functions
	template <typename T>
	bool Vec3<T>::HasNaNs() const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	template <typename T>
	bool Vec3<T>::HasInfinite() const
	{
		return std::isinf(x) || std::isinf(y) || std::isinf(z);
	}

	template <typename T>
	bool Vec3<T>::IsZero(const Vec3& vector)
	{
		return vector.x == 0 && vector.y == 0 && vector.z == 0;
	}

	template <typename T>
	T Vec3<T>::LengthSqrd() const
	{
		return x * x + y * y + z * z;
	}

	template <typename T>
	T Vec3<T>::Length() const
	{
		return std::sqrt(LengthSqrd());
	}

	template <typename T>
	Vec3<T>& Vec3<T>::GetNormalize()
	{
		*this /= Length();
		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Normalize(const Vec3& vector)
	{
		return Vec3(vector.x / vector.Length(), vector.y / vector.Length(), vector.z / vector.Length());
	}

	template <typename T>
	T Vec3<T>::Dot(const Vec3& lhsVector, const Vec3& rhsVector)
	{
		return lhsVector.x * rhsVector.x + lhsVector.y * rhsVector.y + lhsVector.z * rhsVector.z;
	}

	template <typename T>
	T Vec3<T>::AbsDot(const Vec3& lhsVector, const Vec3& rhsVector)
	{
		return std::abs(Dot(lhsVector, rhsVector));
	}

	template <typename T>
	Vec3<T> Vec3<T>::Cross(const Vec3<T>& lhsVector, const Vec3<T>& rhsVector)
	{
		return Vec3(
			(lhsVector.y * rhsVector.z) - (lhsVector.z * rhsVector.y),
			(lhsVector.z * rhsVector.x) - (lhsVector.x * rhsVector.z),
			(lhsVector.x * rhsVector.y) - (lhsVector.y * rhsVector.x));
	}

	template <typename T>
	Vec3<T> Vec3<T>::Lerp(const Vec3& vecOne, const Vec3& vecTwo, T t)
	{
		return (vecOne * (1 - t) + vecTwo * t);
	}

	template <typename T>
	T Vec3<T>::MinComponent(const Vec3& vector)
	{
		return std::min<T>(vector.x, std::min<T>(vector.y, vector.z));
	}

	template <typename T>
	T Vec3<T>::MaxComponent(const Vec3& vector)
	{
		return std::max<T>(vector.x, std::max<T>(vector.y, vector.z));
	}

	template <typename T>
	Vec3<T> Vec3<T>::Min(const Vec3& lhsVector, const Vec3& rhsVector)
	{
		return Vec3(std::min<T>(lhsVector.x, rhsVector.x), std::min<T>(lhsVector.y, rhsVector.y), std::min<T>(lhsVector.z, rhsVector.z));
	}

	template <typename T>
	Vec3<T> Vec3<T>::Max(const Vec3& lhsVector, const Vec3& rhsVector)
	{
		return Vec3(std::max<T>(lhsVector.x, rhsVector.x), std::max<T>(lhsVector.y, rhsVector.y), std::max<T>(lhsVector.z, rhsVector.z));
	}

	template <typename T>
	Vec3<T> Vec3<T>::Permute(const Vec3& vector, int x, int y, int z)
	{
		return Vec3(vector[x], vector[y], vector[z]);
	}

	template <typename T>
	void Vec3<T>::CoordinateSystem(const Vec3& firstVector, Vec3* secondVector, Vec3* thirdVector)
	{
		if (std::abs(firstVector.x) > std::abs(firstVector.y))
		{
			*secondVector = Vec3<T>(-firstVector.z, 0, firstVector.x) / std::sqrt(firstVector.x * firstVector.x + firstVector.z * firstVector.z);
		}
		else
		{
			*secondVector = Vec3<T>(0, firstVector.z, -firstVector.y) / std::sqrt(firstVector.y * firstVector.y + firstVector.z * firstVector.z);
		}

		*thirdVector = Cross(firstVector, secondVector);
	}
}
