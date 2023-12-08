#include "glpch.h"

#include "Line.h"
#include "glad/glad.h"

namespace Galactica
{
	void Line::SetLine(const std::vector<glm::vec3>& vertices)
	{
		this->lineVertices = vertices;

		InitLine();
	}

	void Line::DebugMode(bool set)
	{
	}

	void Line::DrawLine(Shader& shader, int type)
	{
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		switch (type)
		{
		case 1:
			glDrawArrays(GL_LINE_STRIP, 0, lineVertices.size());
			break;
		case 2:
			glDrawArrays(GL_LINES, 0, lineVertices.size());
			break;

		}
		
	}

	void Line::InitLine()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*lineVertices.size(), lineVertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
}

