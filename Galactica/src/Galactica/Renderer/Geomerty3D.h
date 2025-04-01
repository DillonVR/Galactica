#pragma once

#include "Galactica/Math/Mat3.h"
#include "Galactica/Math/Vec3.h"
#include "Galactica/Vendor/glm/glm/geometric.hpp"

namespace Galactica
{
	//Geometry
	typedef Vec3f Point;

	typedef struct Line
	{
		Point start;
		Point end;

		inline Line() {}

		inline Line(const Point& s, const Point& e) :
			start(s), end(e)
		{}

	} Line;

	typedef struct Ray {
		Point origin;
		Vec3f direction;

		inline Ray() : direction(0.0f, 0.0f, 1.0f) {}

		inline Ray(const Point& o, const Vec3f& d) :
			origin(o), direction(d)
		{
			NormalizeDirection();
		}

		inline void NormalizeDirection()
		{
			Vec3f::Normalize(direction);
		}
	} Ray;

	typedef struct OBB
	{
		Point position;
		Galactica::Vec3f size;
		Galactica::Mat3<float> orientation;

		OBB() : size(1, 1, 1)
		{}

		OBB(const Point& p, const Vec3f& s) :
			position(p), size(s)
		{}

		OBB(const Point& p, const Vec3f& s, const Mat3<float>& o) :
			position(p), size(s), orientation(o)
		{}
	} OBB;

	float LengthSq(const Line& line);

	bool LineTest(const OBB& obb, const Line& line);
	

}
