#pragma once

#include <array>

namespace Galactica
{
	template <typename T>
	class Vec3
	{
	public:
		// Declare variables
		union
		{
			struct {
				T x;
				T y;
				T z;
			};

			struct {
				T r;
				T g;
				T b;
			};

			std::array<T, 3> variables;
		};

		constexpr Vec3() noexcept;

		constexpr Vec3(const Vec3& vector) noexcept;

		constexpr Vec3(T x, T y, T z) noexcept;

		constexpr Vec3(T value) noexcept;

		constexpr Vec3(std::initializer_list<T> data) noexcept;

		~Vec3() = default;

		// Operations
		Vec3& operator=(const Vec3& vector);
		Vec3 operator+(const Vec3& vector);
		Vec3& operator+=(const Vec3& vector);
		Vec3 operator-(const Vec3& vector);
		Vec3 operator-(const Vec3& vector) const;
		Vec3& operator-=(const Vec3& vector);
		Vec3 operator*(T value);
		Vec3& operator*=(T value);
		Vec3 operator/(T value);
		Vec3& operator/=(T value);

		bool operator==(const Vec3& vector);
		bool operator!=(const Vec3& vector);

		const T* GetPointerToData() const;

		friend Vec3 operator*(T scalar, const Vec3& vector)
		{
			GL_CORE_ASSERT(!std::isnan(scalar), "Operation '/' failed, the value is NaN!");
			return Vec3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
		}

		friend Vec3 operator*(const Vec3& vector, T scalar)
		{
			GL_CORE_ASSERT(!std::isnan(scalar), "Operation '/' failed, the value is NaN!");
			return Vec3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
		}

		friend std::ostream& operator<<(std::ostream& os, const Vec3& vector)
		{
			os << "[" << vector.x << ", " << vector.y << ", " << vector.z << "]";
			return os;
		}

		bool HasNaNs() const;
		bool HasInfinite() const;

		static bool IsZero(const Vec3& vector);

		T LengthSqrd() const;
		T Length() const;

		Vec3& GetNormalize();
		static Vec3 Normalize(const Vec3& vector);

		static T Dot(const Vec3& lhsVector, const Vec3& rhsVector);
		static T AbsDot(const Vec3& lhsVector, const Vec3& rhsVector);

		static Vec3 Cross(const Vec3& lhsVector, const Vec3& rhsVector);

		static Vec3 Lerp(const Vec3& vecOne, const Vec3& vecTwo, T t);

		static T MinComponent(const Vec3& vector);
		static T MaxComponent(const Vec3& vector);

		Vec3 Min(const Vec3& lhsVector, const Vec3& rhsVector);
		Vec3 Max(const Vec3& lhsVector, const Vec3& rhsVector);
		Vec3 Permute(const Vec3& vector, int x, int y, int z);
	};
	using Vec3f = Vec3<float>;
}

#include "Vec3.inl"