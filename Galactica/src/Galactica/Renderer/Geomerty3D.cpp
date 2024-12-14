#include "glpch.h"

#include "Geomerty3D.h"

namespace Galactica
{
		float LengthSq(const Line& line)
		{
			auto dis = line.end - line.start;
			return dis.LengthSqrd();
		}

		bool Linetest(const OBB& obb, const Line& line)
		{

			Vec3f dis = line.end - line.start;


			/*if (dis.LengthSqrd() < 0.0000001f)
			{
				return PointInOBB(line.start, obb);
			}

			Ray ray;
			ray.origin = line.start;

			ray.direction = dis.GetNormalize();
			RaycastResult result;
			if (!Raycast(obb, ray, &result)) {
				return false;
			}
			float t = result.t;*/

			//return t >= 0 && t * t <= LengthSq(line);
			return true;
		}
}
