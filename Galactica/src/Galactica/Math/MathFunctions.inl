#include "Vec3.h"
#include "MathConstants.h"

namespace Galactica::Math
{
	template <typename T>
	bool NearEquals(const T floatOne, const T floatTwo)
	{
		T absDiff = std::abs(floatOne - floatTwo);
		if (absDiff <= EPSILON)
		{
			return true;
		}

		return false;
	}

	template <typename T>
	T DegreesToRad(const T angleInDegrees)
	{
		return angleInDegrees * static_cast<T>(F_PI / 180.0f);
	}

	template <typename T>
	T RadToDegrees(const T angleInDegrees)
	{
		return angleInDegrees * static_cast<T>(180.0f / F_PI);
	}

	template <typename T>
	T FastInverseSquareRootAroundOne(const T value)
	{
		constexpr T coeffOne = static_cast<T>(15.0f / 8.0f);
		constexpr T coeffTwo = static_cast<T>(-5.0f / 4.0f);
		constexpr T coeffThree = static_cast<T>(3.0f / 8.0f);

		return coeffOne + coeffTwo * value + coeffThree * value * value;
	}

	template <typename T>
	T MapValueInRange(const T value, const T inputStart, const T inputEnd, const T outputStart, const T outputEnd)
	{
		return outputStart + (outputEnd - outputStart) / (inputEnd - inputStart) * (value - inputStart);
	}

	template <typename T>
	Vec3<T> ClosestPointOnLineSegment(const Vec3<T>& lineStartPoint, const Vec3<T>& lineEndPoint, const Vec3<T>& point)
	{
		const Vec3<T> lineDirection = lineEndPoint - lineStartPoint;
		const T t = Vec3<T>::Dot(point - lineStartPoint, lineDirection) / Vec3<T>::Dot(lineDirection, lineDirection);
		return lineStartPoint + Saturate(t) * lineDirection;
	}

	template <typename T>
	T Saturate(T value)
	{
		return std::min(std::max(value, static_cast<T>(0)), static_cast<T>(1));
	}
}