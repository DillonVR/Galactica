#pragma once
#include "Galactica/Vendor/GLFW/src/internal.h"
#include "Galactica/Vendor/glm/glm/glm.hpp"

namespace Galactica
{
	class GALACTICA_API Shader
	{
	public:

        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);

        void use();

        void stop();

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
	};
}
