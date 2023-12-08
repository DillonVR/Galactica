#pragma once

#include <vector>

#include "Vec3.h"
#include "Galactica/Vendor/glm/glm/vec3.hpp"

namespace Galactica
{

	class Spline
	{
	public:
		Spline() = default;

		glm::vec3 get_position(float t, bool closed);

		glm::vec3 get_tangent(float t);

		std::vector<glm::vec3> points;

	private:
		int clampListPos(int pos);

		void calculate_t_values();

		void calculate_knots();

		void create_spline();

		// A vector of control points
		

		// A vector of parameter values for each point
		std::vector<double> t_values;

		// A vector of knots for the spline
		std::vector<double> knots;

		// A flag to indicate if the spline is closed or not
		bool is_closed;

	};
}
