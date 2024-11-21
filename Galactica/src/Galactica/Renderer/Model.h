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

	#define MAX_BONE 100

	struct BoneInfo
	{
		int id;
		glm::mat4 offset;
	};

	struct BoneLine
	{
		glm::vec3 start;
		glm::vec3 end;
	};
	
	class GALACTICA_API Model
	{
	public:

		std::vector<Texture> textureLoaded;
		std::vector<Mesh> meshes;
		std::string directory;
		std::unordered_map<std::string, BoneInfo> boneMap;
		std::vector<BoneLine> boneLines;

		int boneNum = 0;

		bool gammaCorrection;

		Model() = default;

		void DrawModel(Shader &shader);

		void LoadModel(std::string const& path, bool gamma);

		std::unordered_map<std::string, BoneInfo> GetBoneMap() { return boneMap; }

		int& GetBoneCount() { return boneNum; }

		void Bonelines(aiNode* node, BoneLine bone_lines);


		std::vector<BoneLine> GetBoneLines();

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


