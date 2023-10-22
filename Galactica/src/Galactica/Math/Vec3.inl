#pragma once

#include <cassert>
#include <cmath>
#include <string>
#include "MathFunctions.h"
#include "Vec3.h"
#include <sol/sol.hpp>

namespace Galactica::Math
{
	template <typename T>
	Vec3<T>::Vec3()
		: x(0), y(0), z(0)
	{
	}

	template<typename T>
	Vec3<T>::Vec3(std::initializer_list<T> args)
	{
		int index = 0;
		for (auto begin = args.begin(); begin != args.end(); ++begin)
		{
			this->data.at(index++) = *begin;
		}
	}

	template <typename T>
	Vec3<T>::Vec3(const T x, const T y, const T z)
		: x(x), y(y), z(z)
	{
	}

	template <typename T>
	Vec3<T>::Vec3(const T value)
		: x(value), y(value), z(value)
	{
	}

	template <typename T>
	T Vec3<T>::Magnitude() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	template <typename T>
	T Vec3<T>::SqrMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	template <typename T>
	Vec3<T> Vec3<T>::GetNormalize() const
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		T inverseMagnitude = static_cast<T>(1) / Magnitude();

		return Vec3<T>(x * inverseMagnitude, y * inverseMagnitude, z * inverseMagnitude);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::Normalize()
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		*this /= Magnitude();

		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::GetRenormalize() const
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SqrMagnitude());
		return Vec3(x * inverseMagnitude, y * inverseMagnitude, z * inverseMagnitude);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::Renormalize()
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SqrMagnitude());
		return *this *= inverseMagnitude;
	}

	template <typename T>
	Vec3<T> Vec3<T>::GetNegate() const
	{
		return *this * static_cast<T>(-1);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::Negate()
	{
		*this *= static_cast<T>(-1);
		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::GetAbsoluteValue()
	{
		Vec3<T> absoluteValueVector(std::abs(x), std::abs(y), std::abs(z));

		return absoluteValueVector;
	}

	template <typename T>
	T Vec3<T>::GetLargestElement() const
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

		return largest;
	}

	template<typename T>
	const T* Vec3<T>::GetValuePointer() const
	{
		return data.data();
	}

	template <typename T>
	const std::string Vec3<T>::ToString() const
	{
		std::string vectorString = "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
		return vectorString;
	}

	template <typename T>
	T Vec3<T>::Dot(const Vec3& leftVector, const Vec3& rightVector)
	{
		return leftVector.x * rightVector.x +
			   leftVector.y * rightVector.y +
			   leftVector.z * rightVector.z;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Cross(const Vec3& leftVector, const Vec3& rightVector)
	{
		return Vec3(leftVector.y * rightVector.z - leftVector.z * rightVector.y,
					leftVector.z * rightVector.x - leftVector.x * rightVector.z,
					leftVector.x * rightVector.y - leftVector.y * rightVector.x);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Project(const Vec3& leftVector, const Vec3& rightVector)
	{
		return rightVector * (Dot(leftVector, rightVector) / Dot(rightVector, rightVector));
	}

	template <typename T>
	Vec3<T> Vec3<T>::Reject(const Vec3& leftVector, const Vec3& rightVector)
	{
		return leftVector - Project(leftVector, rightVector);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Lerp(const Vec3& leftVector, const Vec3& rightVector, const float t)
	{
		return leftVector * (1.0f - t) + rightVector * t;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Zero()
	{
		return Vec3<T>(0, 0, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::XAxis()
	{
		return Vec3<T>(1, 0, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::YAxis()
	{
		return Vec3<T>(0, 1, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::ZAxis()
	{
		return Vec3<T>(0, 0, 1);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Up()
	{
		return Vec3<T>(0, 1, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Down()
	{
		return Vec3<T>(0, -1, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Left()
	{
		return Vec3<T>(-1, 0, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Right()
	{
		return Vec3<T>(1, 0, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Back()
	{
		return Vec3<T>(0, 0, 1);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Forward()
	{
		return Vec3<T>(0, 0, -1);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator=(const Vec3<T>& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	// Private Functions to expose in Lua
	template <typename T>
	Vec3<T> Vec3<T>::Add(const Vec3& vector)
	{
		return vector + (*this);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Subtract(const Vec3& vector)
	{
		return (*this) - vector;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Multiply(T scalar)
	{
		return (*this) * scalar;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Divide(T scalar)
	{
		return (*this) / scalar;
	}

	template <typename T>
	void Vec3<T>::InitializeClassWithSol(const std::shared_ptr<sol::state>& luaState)
	{
		luaState->new_usertype<Vec3>(
			// Class Name
			"Vec3",

			// Variables
			"x", &Vec3::x,
			"y", &Vec3::y,
			"z", &Vec3::z,

			"new", sol::constructors<
				Vec3(), 
				Vec3(T, T, T), 
				Vec3(T)>(),

			// Meta Functions for overloading and special functions
			sol::meta_function::to_string,		&Vec3::ToString,
			sol::meta_function::addition,		&Vec3::Add,
			sol::meta_function::subtraction,	&Vec3::Subtract,
			sol::meta_function::multiplication, &Vec3::Multiply,
			sol::meta_function::division,		&Vec3::Divide,

			// Supported Functions
			"SquareMagnitude",	&Vec3::SqrMagnitude,
			"Magnitude",		&Vec3::Magnitude,
			"GetNormalize",		&Vec3::GetNormalize,
			"Normalize",		&Vec3::Normalize,
			"GetRenormalize",	&Vec3::GetRenormalize,
			"Renormalize",		&Vec3::Renormalize,
			"GetNegate",		&Vec3::GetNegate,
			"Negate",			&Vec3::Negate,
			"GetAbsoluteValue", &Vec3::GetAbsoluteValue
		);

		// Static Variables
		luaState->set_function("Vec3Cross",		&Vec3::Cross);
		luaState->set_function("Vec3Dot",		&Vec3::Dot);
		luaState->set_function("Vec3Project",	&Vec3::Project);
		luaState->set_function("Vec3Reject",		&Vec3::Reject);
		luaState->set_function("Vec3Lerp",		&Vec3::Lerp);
		luaState->set_function("Vec3Zero",		&Vec3::Zero);
		luaState->set_function("Vec3XAxis",		&Vec3::XAxis);
		luaState->set_function("Vec3YAxis",		&Vec3::YAxis);
		luaState->set_function("Vec3ZAxis",		&Vec3::ZAxis);
		luaState->set_function("Vec3Up",			&Vec3::Up);
		luaState->set_function("Vec3Down",		&Vec3::Down);
		luaState->set_function("Vec3Left",		&Vec3::Left);
		luaState->set_function("Vec3Right",		&Vec3::Right);
		luaState->set_function("Vec3Forward",	&Vec3::Forward);
		luaState->set_function("Vec3Back",		&Vec3::Back);
	}
}
