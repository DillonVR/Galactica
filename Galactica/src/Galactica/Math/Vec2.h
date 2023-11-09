#pragma once

#include <ostream>

namespace Galactica
{
	template <typename T>
	class Vec2
	{
	public:
		// Declare variables
		union
		{
			struct 
			{
				T x, y;
			};

			std::array<T, 2> variables;
		};

		constexpr Vec2() noexcept;

		constexpr Vec2(T x, T y) noexcept;

		constexpr Vec2(T value) noexcept;

		constexpr Vec2(std::initializer_list<T> data) noexcept;

		explicit constexpr Vec2(const Vec2& vector) noexcept;

		~Vec2() = default;

		// Operations
		Vec2& operator=(const Vec2& vector);

		Vec2 operator+(const Vec2& vector);

		Vec2& operator+=(const Vec2& vector);

		Vec2 operator-(const Vec2& vector);

		Vec2& operator-=(const Vec2& vector);

		Vec2 operator*(T value);

		Vec2& operator*=(T value);

		Vec2 operator/(T value);

		Vec2& operator/=(T value);

		bool operator==(const Vec2& vector);

		bool operator!=(const Vec2& vector);

		const T* GetPointerToData() const;

		friend std::ostream& operator<<(std::ostream& os, const Vec2& vector)
		{
			os << "[" << vector.x << ", " << vector.y << "]";
			return os;
		}

		// Core Functions
		bool HasNaNs() const;

		bool HasInfinite() const;

		static bool IsZero(const Vec2& vector);

		T LengthSquared() const;

		T Length() const;

		Vec2& GetNormalize();

		static Vec2 Normalize(const Vec2& vector);

		static T Dot(const Vec2& lhs, const Vec2& rhs);

		static T AbsDot(const Vec2& lhs, const Vec2& rhs);
	};
};

#include "Vec2.inl"
