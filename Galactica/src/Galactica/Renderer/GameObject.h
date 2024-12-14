#pragma once

#include "Galactica/Vendor/glm/glm/fwd.hpp"
#include "Galactica/Vendor/glm/glm/vec3.hpp"

namespace Galactica
{
	class GameObject
	{
	public:
		GameObject()
		{
			pos = glm::vec3(0.0f);
			rot = glm::vec3(0.0f);
			scale = glm::vec3(1.0f);
		}
		~GameObject() {}

		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 scale;

		glm::mat4 getTransform() const;
	private:
	};

}

