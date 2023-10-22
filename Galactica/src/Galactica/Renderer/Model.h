#pragma once

#include <map>
#include <assimp/Importer.hpp>

#include "Mesh.h"
#include "Shader.h"

#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

namespace Galactica
{
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

	struct BoneInfo
	{
		int id;
		glm::mat4 offset;
	};

	class GALACTICA_API Model
	{
	public:

		std::vector<Texture> textureLoaded;
		std::vector<Mesh> meshes;
		std::string directory;
		std::map<std::string, BoneInfo> boneMap;

		int boneNum = 0;

		bool gammaCorrection;

		Model() = default;

		void DrawModel(Shader &shader);

		void LoadModel(std::string const& path, bool gamma);

		auto& GetBoneMap() { return boneMap; }

		int& GetBoneCount() { return boneNum; }

	private:

		void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

		void SetVertextBoneDataDefault(Vertex& vertex);

		void processNode(aiNode* node, const aiScene* scene);

		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
}


