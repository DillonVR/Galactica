#pragma once

#include <cassert>
#include <cmath>
#include <string>
#include "MathFunctions.h"
#include "Vec4.h"

namespace Galactica::Math
{
	template <typename T>
	Vec4<T>::Vec4()
		: x(0), y(0), z(0), w(0)
	{
	}

	template <typename T>
	Vec4<T>::Vec4(const Vec4<T>& vector)
		: x(vector.x), y(vector.y), z(vector.z), w(vector.w)
	{
	}

	template<typename T>
	Vec4<T>::Vec4(std::initializer_list<T> args)
	{
		int index = 0;
		for (auto begin = args.begin(); begin != args.end(); ++begin)
		{
			this->data.at(index++) = *begin;
		}
	}

	template <typename T>
	Vec4<T>::Vec4(const T x, const T y, const T z, const T w)
		: x(x), y(y), z(z), w(w)
	{
	}

	template <typename T>
	Vec4<T>::Vec4(const T x, const T y, const T z)
		: x(x), y(y), z(z), w(0)
	{
	}

	template <typename T>
	Vec4<T>::Vec4(const T scalar)
		: x(scalar), y(scalar), z(scalar), w(scalar)
	{
	}

	template <typename T>
	T Vec4<T>::Magnitude() const
	{
		return std::sqrt(SqrMagnitude());
	}

	template <typename T>
	T Vec4<T>::SqrMagnitude() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <typename T>
	Vec4<T> Vec4<T>::GetNormalize() const
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		T magnitude = Magnitude();

		return Vec4<T>(x / magnitude, y / magnitude, z / magnitude, w / magnitude);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::Normalize()
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		*this /= Magnitude();

		return *this;
	}

	template <typename T>
	Vec4<T> Vec4<T>::GetRenormalize() const
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SqrMagnitude());
		return Vec4<T>(x * inverseMagnitude, y * inverseMagnitude, z * inverseMagnitude, w * inverseMagnitude);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::Renormalize()
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SqrMagnitude());
		return *this *= inverseMagnitude;
	}

	template <typename T>
	Vec4<T> Vec4<T>::GetNegate() const
	{
		return *this * static_cast<T>(-1);
	}

	template <typename T>
	Vec4<T>& Vec4<T>::Negate()
	{
		*this *= static_cast<T>(-1);
		return *this;
	}

	template <typename T>
	Vec4<T> Vec4<T>::GetAbsoluteValue()
	{
		Vec4<T> absoluteValueVector(std::abs(x), std::abs(y), std::abs(z), std::abs(w));

		return absoluteValueVector;
	}

	template <typename T>
	T Vec4<T>::GetLargestElement() const
	{
		float largest = x;

		if (y > largest)
		{
			largest = y;
		}

		if (z > largest)
		{
			largest = z;
		}

		if (w > largest)
		{
			largest = w;
		}

		return largest;
	}

	template<typename T>
	const T* Vec4<T>::GetValuePointer() const
	{
		return data.data();
	}

	template <typename T>
	std::string Vec4<T>::ToString() const
	{
		std::string vectorString = "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
		return vectorString;
	}

	template <typename T>
	T Vec4<T>::Dot(const Vec4<T>& leftVector, const Vec4<T>& rightVector)
	{
		return leftVector.x * rightVector.x +
			   leftVector.y * rightVector.y +
			   leftVector.z * rightVector.z +
			   leftVector.w * rightVector.w;
	}

	template <typename T>
	Vec4<T> Vec4<T>::Lerp(const Vec4<T>& leftVector, const Vec4<T>& rightVector, const T t)
	{
		return leftVector * (static_cast<T>(1) - t) + rightVector * t;
	}

	template <typename T>
	Vec4<T> Vec4<T>::Zero()
	{
		return Vec4<T>();
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator=(const Vec4<T>& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = vector.w;
		return *this;
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;
		return *this;
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;
		return *this;
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	template <typename T>
	void Vec4<T>::InitializeClassWithSol(const std::shared_ptr<sol::state>& luaState)
	{
		luaState->new_usertype<Vec4>(
			// Class Name
			"Vec4",

			// Variables
			"x", &Vec4::x,
			"y", &Vec4::y,
			"z", &Vec4::z,
			"w", &Vec4::w,

			"new", sol::constructors<
				Vec4(), 
				Vec4(T, T, T),
				Vec4(T, T, T, T), 
				Vec4(T)>(),

			// Meta Functions for overloading and special functions
			sol::meta_function::to_string,		&Vec4::ToString,
			sol::meta_function::addition,		&Vec4::Add,
			sol::meta_function::subtraction,	&Vec4::Subtract,
			sol::meta_function::multiplication, &Vec4::Multiply,
			sol::meta_function::division,		&Vec4::Divide,

			// Supported Functions
			"SquareMagnitude",	&Vec4::SqrMagnitude,
			"Magnitude",		&Vec4::Magnitude,
			"GetNormalize",		&Vec4::GetNormalize,
			"Normalize",		&Vec4::Normalize,
			"GetRenormalize",	&Vec4::GetRenormalize,
			"Renormalize",		&Vec4::Renormalize,
			"GetNegate",		&Vec4::GetNegate,
			"Negate",			&Vec4::Negate,
			"GetAbsoluteValue", &Vec4::GetAbsoluteValue
		);

		// Static Variables
		luaState->set_function("Vec4Dot",	&Vec4::Dot);
		luaState->set_function("Vec4Lerp",	&Vec4::Lerp);
		luaState->set_function("Vec4Zero",	&Vec4::Zero);
	}

	template <typename T>
	Vec4<T> Vec4<T>::Add(const Vec4& vector)
	{
		return vector + (*this);
	}

	template <typename T>
	Vec4<T> Vec4<T>::Subtract(const Vec4& vector)
	{
		return vector - (*this);
	}

	template <typename T>
	Vec4<T> Vec4<T>::Multiply(T scalar)
	{
		return (*this) * scalar;
	}

	template <typename T>
	Vec4<T> Vec4<T>::Divide(T scalar)
	{
		return (*this) / scalar;
	}
}
