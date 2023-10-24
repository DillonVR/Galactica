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
	    
	    int m_BoneIDs[MAX_BONE_INFLUENCE];
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

		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

		void lineMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

		//void lineBone(const std::vector<Galactica::BoneLine>& vertices);

		void DebugMode(bool set);

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


