#pragma once
#include "Galactica/Vendor/GLFW/src/internal.h"
#include "Galactica/Vendor/glm/glm/glm.hpp"

namespace Galactica
{
	class GALACTICA_API Shader
	{
	public:
		unsigned int ID;
		Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		void setMat4(const std::string& name, const glm::mat4& mat) const;
		void setFloat(const std::string& name, float value) const;

		void Use();
	private:
		void checkCompileError(GLuint shader, std::string type);
	};
}
