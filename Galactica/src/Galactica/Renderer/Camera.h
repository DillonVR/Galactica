#pragma once

#include "Galactica/Vendor/glm/glm/glm.hpp"

namespace Galactica {

	class Camera
	{
	public:
		Camera() = default;

		Camera(const glm::mat4& projection)
			: cameraProjection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const
		{
			return cameraProjection;
		}

	protected:
		glm::mat4 cameraProjection = glm::mat4(1.0f);
	};

}
