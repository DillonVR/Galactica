#pragma once

#include <assimp/Importer.hpp>

#include "Mesh.h"
#include "Shader.h"

#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "GameObject.h"

namespace Galactica
{
    class GALACTICA_API Model : public GameObject
	{
	    public:
	        Model(std::string path)
    		{
	            loadModel(path);
	        }

	        ~Model() {}

	        glm::vec3 color;

	        void draw(Shader& shader);

	        std::vector<Mesh> meshes;

	        std::string directory;

	    private:

	        void loadModel(std::string path);

	        void processNode(aiNode* node, const aiScene* scene);

	        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    };
}


