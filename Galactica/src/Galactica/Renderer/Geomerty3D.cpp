#include "glpch.h"

#include "Geomerty3D.h"

namespace Galactica
{
		float LengthSq(const Line& line)
		{
			auto dis = line.end - line.start;
			return dis.LengthSqrd();
		}

		bool LineTest(const OBB& obb, const Line& line)
		{

			Vec3f dis = line.end - line.start;
			return true;
		}
}
