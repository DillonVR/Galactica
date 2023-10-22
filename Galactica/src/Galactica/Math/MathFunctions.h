#pragma once

namespace Galactica::Math
{
	template <typename T> class Vec3;

	template <typename T>
	inline bool NearEquals(const T floatOne, const T floatTwo);

	template <typename T>
	inline T DegreesToRad(const T angleInDegrees);

	template <typename T>
	inline T RadToDegrees(const T angleInDegrees);

	template <typename T>
	inline T FastInverseSquareRootAroundOne(const T value);

	template <typename T>
	inline T MapValueInRange(const T value, const T inputStart, const T inputEnd, const T outputStart, const T outputEnd);

	template <typename T>
	inline Vec3<T> ClosestPointOnLineSegment(const Vec3<T>& lineStartPoint, const Vec3<T>& lineEndPoint, const Vec3<T>& point);

	template <typename T>
	inline T Saturate(T value);
}

#include "MathFunctions.inl"
