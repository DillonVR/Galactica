#pragma once

#include <cassert>
#include <cmath>
#include <string>
#include <sol/sol.hpp>

#include "Vec2.h"

namespace Galactica::Math
{
	template <typename T>
	Vec2<T>::Vec2()
		: x(0), y(0)
	{
	}

	template<typename T>
	Vec2<T>::Vec2(std::initializer_list<T> args)
	{
		int index = 0;
		for (auto begin = args.begin(); begin != args.end(); ++begin)
		{
			this->data.at(index++) = *begin;
		}
	}

	template <typename T>
	Vec2<T>::Vec2(const T x, const T y)
		: x(x), y(y)
	{
	}

	template <typename T>
	Vec2<T>::Vec2(const T scalar)
		: x(scalar), y(scalar)
	{
	}

	template <typename T>
	Vec2<T>::Vec2(const Vec2& vector)
		: x(vector.x), y(vector.y)
	{
	}

	template <typename T>
	Vec2<T> Vec2<T>::Zero()
	{
		return Vec2<T>();
	}

	template <typename T>
	Vec2<T> Vec2<T>::XAxis()
	{
		return Vec2<T>(1, 0);
	}

	template <typename T>
	Vec2<T> Vec2<T>::YAxis()
	{
		return Vec2<T>(0, 1);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Up()
	{
		return Vec2<T>(0, 1);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Down()
	{
		return Vec2<T>(0, -1);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Left()
	{
		return Vec2<T>(-1, 0);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Right()
	{
		return Vec2<T>(1, 0);
	}

	template <typename T>
	T Vec2<T>::Magnitude() const
	{
		return std::sqrt(SqrMagnitude());
	}

	template <typename T>
	T Vec2<T>::SqrMagnitude() const
	{
		return x * x + y * y;
	}

	template <typename T>
	Vec2<T> Vec2<T>::GetNormalize() const
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		T inverseMagnitude = static_cast<T>(1) / Magnitude();

		return Vec2<T>(x * inverseMagnitude, y * inverseMagnitude);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Normalize()
	{
		assert(!NearEquals(Magnitude(), static_cast<T>(0)));

		*this /= Magnitude();

		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::GetRenormalize() const
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SqrMagnitude());
		return Vec2<T>(x * inverseMagnitude, y * inverseMagnitude);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Renormalize()
	{
		T inverseMagnitude = FastInverseSquareRootAroundOne(SqrMagnitude());
		return *this *= inverseMagnitude;
	}

	template <typename T>
	Vec2<T> Vec2<T>::GetNegate() const
	{
		return *this * static_cast<T>(-1);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Negate()
	{
		*this *= static_cast<T>(-1);
		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::GetAbsoluteValue()
	{
		Vec2<T> absoluteValueVector(std::abs(x), std::abs(y));

		return absoluteValueVector;
	}

	template <typename T>
	T Vec2<T>::GetLargestElement() const
	{
		if (x > y)
		{
			return x;
		}

		return y;
	}

	template<typename T>
	const T* Vec2<T>::GetValuePointer() const
	{
		return data.data();
	}

	template <typename T>
	const std::string Vec2<T>::ToString() const
	{
		std::string vectorString = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
		return vectorString;
	}

	template <typename T>
	T Vec2<T>::Dot(const Vec2<T>& leftVector, const Vec2<T>& rightVector)
	{
		return leftVector.x * rightVector.x + leftVector.y * rightVector.y;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Project(const Vec2<T>& leftVector, const Vec2<T>& rightVector)
	{
		return rightVector * (Dot(leftVector, rightVector) / Dot(rightVector, rightVector));
	}

	template <typename T>
	Vec2<T> Vec2<T>::Reject(const Vec2<T>& leftVector, const Vec2<T>& rightVector)
	{
		return leftVector - Project(leftVector, rightVector);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Lerp(const Vec2<T>& leftVector, const Vec2<T>& rightVector, const T t)
	{
		return (leftVector * (static_cast<T>(1) - t)) + (rightVector * t);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator=(const Vec2<T>& vector)
	{
		x = vector.x;
		y = vector.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& vector)
	{
		x += vector.x;
		y += vector.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& vector)
	{
		x -= vector.x;
		y -= vector.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	// Private Functions to expose in Lua
	template <typename T>
	Vec2<T> Vec2<T>::Add(const Vec2& vector)
	{
		return vector + (*this);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Subtract(const Vec2& vector)
	{
		return (*this) - vector;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Multiply(const Vec2& vector)
	{
		return (*this) * vector;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Multiply(T scalar)
	{
		return (*this) * scalar;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Divide(T scalar)
	{
		return (*this) / scalar;
	}

	template <typename T>
	void Vec2<T>::InitializeClassWithSol(const std::shared_ptr<sol::state>& luaState)
	{
		luaState->new_usertype<Vec2>(
			// Class Name
			"Vec2",

			// Variables
			"x", &Vec2::x,
			"y", &Vec2::y,

			// Constructors
			"new", sol::constructors<
				Vec2(), 
				Vec2(T, T), 
				Vec2(T), 
				Vec2(const Vec2&)>(),

			// Meta Functions for overloading and special functions
			sol::meta_function::to_string,		&Vec2::ToString,
			sol::meta_function::addition,		&Vec2::Add,
			sol::meta_function::subtraction,	&Vec2::Subtract,

			sol::meta_function::multiplication, sol::overload(
				sol::resolve<Vec2(const Vec2&)>(&Vec2::Multiply), 
				sol::resolve<Vec2(T)>(&Vec2::Multiply)),

			sol::meta_function::division,		&Vec2::Divide,

			// Supported Functions
			"SquareMagnitude",	&Vec2::SqrMagnitude,
			"Magnitude",		&Vec2::Magnitude,
			"GetNormalize",		&Vec2::GetNormalize,
			"Normalize",		&Vec2::Normalize,
			"GetRenormalize",	&Vec2::GetRenormalize,
			"Renormalize",		&Vec2::Renormalize,
			"GetNegate",		&Vec2::GetNegate,
			"Negate",			&Vec2::Negate,
			"GetAbsoluteValue", &Vec2::GetAbsoluteValue
		);

		// Static Variables
		luaState->set_function("Dot",		&Vec2::Dot);
		luaState->set_function("Project",	&Vec2::Project);
		luaState->set_function("Reject",		&Vec2::Reject);
		luaState->set_function("Lerp",		&Vec2::Lerp);
		luaState->set_function("Zero",		&Vec2::Zero);
		luaState->set_function("XAxis",		&Vec2::XAxis);
		luaState->set_function("YAxis",		&Vec2::YAxis);
		luaState->set_function("Up",			&Vec2::Up);
		luaState->set_function("Down",		&Vec2::Down);
		luaState->set_function("Left",		&Vec2::Left);
		luaState->set_function("Right",		&Vec2::Right);
	}
}
