#include "glpch.h"

#include "Model.h"
#include "Galactica/Vendor/stb_image/stb_image.h"

#include "glad/glad.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Galactica/Animation/Animator.h"
#include "Galactica/Animation/AssimpToGLM.h"

namespace Galactica
{
    
	void Model::DrawModel(Shader& shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].DrawMesh(shader);
	}

    // read file via ASSIMP
    void Model::LoadModel(std::string const& path, bool gamma)
	{
		Assimp::Importer importer;
        //Animator::debugBone.resize(MAX_BONE);
        GL_LOGGER_INFO("Assimp : Reading file");
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
        GL_LOGGER_INFO("Assimp :Processing Node Done");
	}

    //Setter for the vertex bone data
    void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            if (vertex.m_BoneIDs[i] < 0)
            {
                vertex.m_Weights[i] = weight;
                vertex.m_BoneIDs[i] = boneID;
                break;
            }
        }
    }

    //Extract Bone Weight For Vertices
    void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
    {
        auto& boneInfoMap = boneMap;
        int& boneCount = boneNum;

        GL_LOGGER_INFO("Assimp :Extracking Bone");

        for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
        {
            int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                BoneInfo newBoneInfo;
                newBoneInfo.id = boneCount;
                newBoneInfo.offset =  AssimpToGLMH::ConvertMatrixToGLMFormat(
                    mesh->mBones[boneIndex]->mOffsetMatrix);
                boneInfoMap[boneName] = newBoneInfo;
                boneID = boneCount;
                boneCount++;
            }
            else
            {
                boneID = boneInfoMap[boneName].id;
            }
            assert(boneID != -1);
            auto weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                assert(vertexId <= vertices.size());
                SetVertexBoneData(vertices[vertexId], boneID, weight);
            }
        }
        GL_LOGGER_INFO("Assimp : Bone Extracting Done");
    }
    //Defalaut Bone data
    void Model::SetVertextBoneDataDefault(Vertex& vertex)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            vertex.m_BoneIDs[i] = -1;
            vertex.m_Weights[i] = 0.0f;
        }
    }

    // process each mesh located at the current node
    // and after we've processed all of the meshes we then recursively process each of the children nodes
	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
            GL_LOGGER_INFO("Assimp :Processing Node -> Mesh");
		}
		
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

    // Fill the data then walk through each of the mesh's vertices
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        GL_LOGGER_INFO("Assimp :Processing Mesh");

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;

            SetVertextBoneDataDefault(vertex);

            vertex.Position = AssimpToGLMH::GetGLMVec(mesh->mVertices[i]);
            vertex.Normal = AssimpToGLMH::GetGLMVec(mesh->mNormals[i]);

            glm::vec3 vector;
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) 
            {
                glm::vec2 vec;
               
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        ExtractBoneWeightForVertices(vertices, mesh, scene);
        GL_LOGGER_INFO("Assimp :Processing Mesh Done");

        return Mesh(vertices, indices, textures);
	}

    //load in the textures
	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < textureLoaded.size(); j++)
            {
                if (std::strcmp(textureLoaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textureLoaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {   
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory,false);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textureLoaded.push_back(texture);
            }
        }
        return textures;
	}

	void Model::Bonelines(aiNode* node, BoneLine bone_lines)
	{
		if (auto parentNode = node->mParent)
        {
            bone_lines.start = { node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c4 };
            bone_lines.end = { parentNode->mTransformation.a4, parentNode->mTransformation.b4, parentNode->mTransformation.c4 };

            boneLines.push_back(bone_lines);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
           Bonelines(node->mChildren[i], bone_lines);
        }
	}

	std::vector<BoneLine> Model::GetBoneLines()
	{
        return boneLines;
	}

	//Read in a Texture from File
	unsigned TextureFromFile(const char* path, const std::string& directory, bool gamma)
	{
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
	}
}
