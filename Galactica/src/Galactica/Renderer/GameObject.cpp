#include "glpch.h"

#include "GameObject.h"
#include <Galactica/Vendor/glm/glm/ext/matrix_transform.hpp>


namespace Galactica
{
	glm::mat4 GameObject::getTransform() const
	{
		glm::mat4 I = glm::mat4(1.0f);
		glm::mat4 T = glm::translate(I, pos);
		glm::mat4 Rx = glm::rotate(I, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 Ry = glm::rotate(I, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 Rz = glm::rotate(I, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 S = glm::scale(I, scale);

		return T * Rz * Ry * Rx * S;
	}
}