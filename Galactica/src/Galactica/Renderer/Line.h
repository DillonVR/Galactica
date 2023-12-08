#pragma once

#include "Shader.h"


namespace Galactica
{
	class GALACTICA_API Line
	{
	public:

		Line() = default;

		void SetLine(const std::vector<glm::vec3>& vertices);

		void DebugMode(bool set);

		void DrawLine(Shader& shader, int type);

		std::vector<glm::vec3> lineVertices;
		
		unsigned int VAO;
	private:

		unsigned int VBO;

		void InitLine();
	};
}


