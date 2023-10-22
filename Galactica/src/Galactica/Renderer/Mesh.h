#pragma once

#include "Shader.h"

#include <vector>
#include <string>

#include "Galactica/Vendor/glm/glm/vec2.hpp"
#include "Galactica/Vendor/glm/glm/vec3.hpp"

namespace Galactica{

	#define MAX_BONE_INFLUENCE 4

	struct Vertex {

	    glm::vec3 Position;
	    glm::vec3 Normal;
	    glm::vec2 TexCoords;
	    glm::vec3 Tangent;
	    glm::vec3 Bitangent;
	    //bone indexes which will influence this vertex
	    int m_BoneIDs[MAX_BONE_INFLUENCE];
	    //weights from each bone
	    float m_Weights[MAX_BONE_INFLUENCE];
	};

	struct Texture {
	    unsigned int id;
	    std::string type;
	    std::string path;
	};



	class GALACTICA_API Mesh
	{
	public:

        Mesh() = default;
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void lineMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		void renderline(bool set);

        void DrawMesh(Shader& shader);

		std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        unsigned int VAO;

    private:
        unsigned int VBO;
        unsigned int EBO;

        void InitMesh();
	};
}


