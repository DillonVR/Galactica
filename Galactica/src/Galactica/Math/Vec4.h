#pragma once

namespace Galactica
{
	template <typename T>
	class Vec3;

	template <typename T>
	class Vec4
	{
	public:
		// Declare variables
		union
		{
			struct
			{
				T x;
				T y;
				T z;
				T w;
			};

			struct
			{
				T r;
				T g;
				T b;
				T a;
			};

			std::array<T, 4> variables;
		};

		constexpr Vec4() noexcept;

		constexpr Vec4(T x, T y, T z, T w) noexcept;

		constexpr Vec4(T value) noexcept;

		constexpr Vec4(std::initializer_list<T> data) noexcept;

		explicit constexpr Vec4(const Vec4& vector);

		constexpr Vec4(const Vec3<T>& vector, T value);

		~Vec4() = default;

		// Operations
		Vec4& operator=(const Vec4& vector);

		Vec4 operator+(const Vec4& vector);

		Vec4& operator+=(const Vec4& vector);

		Vec4 operator-(const Vec4& vector);

		Vec4& operator-=(const Vec4& vector);

		Vec4 operator*(T value);

		Vec4& operator*=(T value);

		Vec4 operator/(T value);

		Vec4& operator/=(T value);

		bool operator==(const Vec4& vector);

		bool operator!=(const Vec4& vector);

		const T* GetPointerToData() const;

		friend std::ostream& operator<<(std::ostream& os, const Vec4& vector)
		{
			os << "[" << vector.x << ", " << vector.y << ", " << vector.z << "]";
			return os;
		}

		// Core Functions
		bool HasNaNs() const;

		bool HasInfinite() const;

		static bool IsZero(const Vec4& vector);

		T LengthSqrd() const;

		T Length() const;

		Vec4& GetNormalize();

		static Vec4 Normalize(const Vec4& vector);

		static T Dot(const Vec4& lhsVector, const Vec4& rhsVector);

		static T AbsDot(const Vec4& lhsVector, const Vec4& rhsVector);

		// Misc Operations
		T MinComponent(const Vec4& vector);

		T MaxComponent(const Vec4& vector);

		Vec4 Min(const Vec4& lhsVector, const Vec4& rhsVector);

		Vec4 Max(const Vec4& lhsVector, const Vec4& rhsVector);

		Vec4 Permute(const Vec4& vector, int x, int y, int z, int w);
	};

	using Vec4f = Vec4<float>;
}

#include "Vec4.inl"
