#pragma once

#include "Vec4.h"

namespace Galactica
{
	template <typename T>
	constexpr Vec4<T>::Vec4() noexcept
		: x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0), w(static_cast<T>(0)))
	{
	}

	template <typename T>
	constexpr Vec4<T>::Vec4(T x, T y, T z, T w) noexcept
		: x(x), y(y), z(z), w(w)
	{
	}

	template <typename T>
	constexpr Vec4<T>::Vec4(T value) noexcept
		: x(value), y(value), z(value), w(value)
	{
	}

	template <typename T>
	constexpr Vec4<T>::Vec4(std::initializer_list<T> data) noexcept
	{
		GL_CORE_ASSERT(data.size() == 4, "Initializer List needs 4 arguments!");
		std::copy(data.begin(), data.end(), variables.begin());
	}

	template <typename T>
	constexpr Vec4<T>::Vec4(const Vec4& vector)
		: x(vector.x), y(vector.y), z(vector.z), w(vector.w)
	{
	}

	template <typename T>
	constexpr Vec4<T>::Vec4(const Vec3<T>& vector, T value)
	{
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;
		this->w = value;
	}

	// Operations
	template <typename T>
	Vec4<T>& Vec4<T>::operator=(const Vec4& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = vector.w;
		return *this;
	}

	template <typename T>
	Vec4<T> Vec4<T>::operator+(const Vec4& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vec4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator+=(const Vec4& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;
		return *this;
	}

	template <typename T>
	Vec4<T> Vec4<T>::operator-(const Vec4& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vec4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator-=(const Vec4& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;
		return *this;
	}

	template <typename T>
	Vec4<T> Vec4<T>::operator*(T value)
	{
		GL_CORE_ASSERT(!std::isnan(value), "Operation '/' failed, the value is NaN!");
		return Vec4(x * value, y * value, z * value, w * value);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator*=(T value)
	{
		x *= value;
		y *= value;
		z *= value;
		w *= value;
		return *this;
	}

	template <typename T>
	Vec4<T> Vec4<T>::operator/(T value)
	{
		GL_CORE_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		return Vec4(x * inverse, y * inverse, z * inverse, w * inverse);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator/=(T value)
	{
		GL_CORE_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		x *= inverse;
		y *= inverse;
		z *= inverse;
		w *= inverse;
		return *this;
	}

	template <typename T>
	bool Vec4<T>::operator==(const Vec4& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
	}

	template <typename T>
	bool Vec4<T>::operator!=(const Vec4& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x != vector.x || y != vector.y || z != vector.z || w != vector.w;
	}

	template <typename T>
	const T* Vec4<T>::GetPointerToData() const
	{
		return variables.data();
	}

	// Core Functions
	template <typename T>
	bool Vec4<T>::HasNaNs() const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w);
	}

	template <typename T>
	bool Vec4<T>::HasInfinite() const
	{
		return std::isinf(x) || std::isinf(y) || std::isinf(z) || std::isinf(w);
	}

	template <typename T>
	bool Vec4<T>::IsZero(const Vec4& vector)
	{
		return vector.x == 0 && vector.y == 0 && vector.z == 0 && vector.w == 0;
	}

	template <typename T>
	T Vec4<T>::LengthSqrd() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <typename T>
	T Vec4<T>::Length() const
	{
		return std::sqrt(LengthSqrd());
	}

	template <typename T>
	Vec4<T>& Vec4<T>::GetNormalize()
	{
		*this /= Length();
		return *this;
	}

	template <typename T>
	Vec4<T> Vec4<T>::Normalize(const Vec4& vector)
	{
		return Vec4(vector.x / vector.Length(), vector.y / vector.Length(), vector.z / vector.Length(), vector.w / vector.Length());
	}

	template <typename T>
	T Vec4<T>::Dot(const Vec4& lhsVector, const Vec4& rhsVector)
	{
		return lhsVector.x * rhsVector.x + lhsVector.y * rhsVector.y + lhsVector.z * rhsVector.z + lhsVector.w * rhsVector.w;
	}

	template <typename T>
	T Vec4<T>::AbsDot(const Vec4& lhsVector, const Vec4& rhsVector)
	{
		return std::abs(Dot(lhsVector, rhsVector));
	}

	template <typename T>
	T Vec4<T>::MinComponent(const Vec4& vector)
	{
		return std::min<T>(vector.x, std::min<T>(vector.y, std::min<T>(vector.z, vector.w)));
	}

	template <typename T>
	T Vec4<T>::MaxComponent(const Vec4& vector)
	{
		return std::max<T>(vector.x, std::max<T>(vector.y, std::max<T>(vector.z, vector.w)));
	}

	template <typename T>
	Vec4<T> Vec4<T>::Min(const Vec4& lhsVector, const Vec4& rhsVector)
	{
		return Vec4(
			std::min<T>(lhsVector.x, rhsVector.x),
			std::min<T>(lhsVector.y, rhsVector.y),
			std::min<T>(lhsVector.z, rhsVector.z),
			std::min<T>(lhsVector.w, rhsVector.w));
	}

	template <typename T>
	Vec4<T> Vec4<T>::Max(const Vec4& lhsVector, const Vec4& rhsVector)
	{
		return Vec4(
			std::max<T>(lhsVector.x, rhsVector.x),
			std::max<T>(lhsVector.y, rhsVector.y),
			std::max<T>(lhsVector.z, rhsVector.z),
			std::max<T>(lhsVector.w, rhsVector.w));
	}

	template <typename T>
	Vec4<T> Vec4<T>::Permute(const Vec4& vector, int x, int y, int z, int w)
	{
		return Vec4(vector[x], vector[y], vector[z], vector[w]);
	}
}