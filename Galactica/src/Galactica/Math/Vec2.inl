#pragma once

#include "Vec2.h"

namespace Galactica
{
	template <typename T>
	constexpr Vec2<T>::Vec2() noexcept
		: x(static_cast<T>(0)), y(static_cast<T>(0))
	{
	}

	template <typename T>
	constexpr Vec2<T>::Vec2(T x, T y) noexcept
		: x(x), y(y)
	{
	}

	template <typename T>
	constexpr Vec2<T>::Vec2(T value) noexcept
		: x(value), y(value)
	{
	}

	template <typename T>
	constexpr Vec2<T>::Vec2(std::initializer_list<T> data) noexcept
	{
		GL_CORE_ASSERT(data.size() == 2, "Initializer List needs 2 arguments!");
		std::copy(data.begin(), data.end(), variables.begin());
	}

	template <typename T>
	constexpr Vec2<T>::Vec2(const Vec2& vector) noexcept
		: x(vector.x), y(vector.y)
	{
	}

	// Operations
	template <typename T>
	Vec2<T>& Vec2<T>::operator=(const Vec2& vector)
	{
		x = vector.x;
		y = vector.y;
		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator+(const Vec2& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vec2(x - vector.x, y - vector.y);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator+=(const Vec2& vector)
	{
		x += vector.x;
		y += vector.y;
		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator-(const Vec2& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vec2(x - vector.x, y - vector.y);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator-=(const Vec2& vector)
	{
		x -= vector.x;
		y -= vector.y;
		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator*(T value)
	{
		GL_CORE_ASSERT(!std::isnan(value), "Operation '/' failed, the value is NaN!");
		return Vec2(x * value, y * value);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator*=(T value)
	{
		x *= value;
		y *= value;
		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator/(T value)
	{
		GL_CORE_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		return Vec2(x * inverse, y * inverse);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator/=(T value)
	{
		GL_CORE_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		x *= inverse;
		y *= inverse;
		return *this;
	}

	template <typename T>
	bool Vec2<T>::operator==(const Vec2& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x == vector.x && y == vector.y;
	}

	template <typename T>
	bool Vec2<T>::operator!=(const Vec2& vector)
	{
		GL_CORE_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x != vector.x || y != vector.y;
	}

	template <typename T>
	const T* Vec2<T>::GetPointerToData() const
	{
		return variables.data();
	}

	// Core Functions
	template <typename T>
	bool Vec2<T>::HasNaNs() const
	{
		return std::isnan(x) || std::isnan(y);
	}

	template <typename T>
	bool Vec2<T>::HasInfinite() const
	{
		return std::isinf(x) || std::isinf(y);
	}

	template <typename T>
	bool Vec2<T>::IsZero(const Vec2& vector)
	{
		return vector.x == 0 && vector.y == 0;
	}

	template <typename T>
	T Vec2<T>::LengthSquared() const
	{
		return x * x + y * y;
	}

	template <typename T>
	T Vec2<T>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template <typename T>
	Vec2<T>& Vec2<T>::GetNormalize()
	{
		*this /= Length();
		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Normalize(const Vec2& vector)
	{
		return Vec2(vector.x / vector.Length(), vector.y / vector.Length());
	}

	template <typename T>
	T Vec2<T>::Dot(const Vec2& lhs, const Vec2& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	template <typename T>
	T Vec2<T>::AbsDot(const Vec2& lhs, const Vec2& rhs)
	{
		return std::abs(Dot(lhs, rhs));
	}
}