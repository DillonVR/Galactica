#pragma once

#include "Shader.h"

#include <vector>
#include <string>

#include "Galactica/Vendor/glm/glm/vec2.hpp"
#include "Galactica/Vendor/glm/glm/vec3.hpp"

namespace Galactica{

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 texCoords;
	};

	struct Texture
	{
		unsigned int id;
		std::string type;
	};

	class GALACTICA_API Mesh
	{
		public:
			std::vector<Vertex> vertices;

			std::vector<unsigned int> indices;

			std::vector<Texture> textures;

			Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

			void draw(Shader& shader);

			void UpdateVertices(std::vector<Vertex> vertices);

		private:
			unsigned int VAO, VBO, EBO;

			void setup();
	};
}


