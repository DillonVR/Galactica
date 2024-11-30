#include "glpch.h"

#include "Animation.h"

#include "AssimpToGLM.h"
#include "assimp/postprocess.h"
#include <queue>

namespace Galactica
{
	//Use Assimp to Load in animations
	void Animation::LoadAnimation(const std::string& animationPath, Model* model)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);

		assert(scene && scene->mRootNode);

		const auto animation = scene->mAnimations[0];

		duration = static_cast<float>(animation->mDuration);

		ticksPerSecond = static_cast<int>(animation->mTicksPerSecond);

		rootNode = new AssimpNodeData();

		ReadHeirarchyData(rootNode, scene->mRootNode);
		ReadMissingBones(animation, *model);
	}

	Animation::~Animation()
	{
		
	}

	//Find Bone if bone map has it.
	Bone* Animation::FindBone(const std::string& name)
	{
		auto iter = std::find_if(bones.begin(), bones.end(),
			[&](const Bone& Bone)
			{
				return Bone.GetBoneName() == name;
			}
		);
		if (iter == bones.end())
		{
			return nullptr;
		}
		return &(*iter);
	}

	
	std::vector<AssimpNodeData*> Animation::GetManipulators(std::string const& endEffeactor)
	{
		auto manipulator = GetNodeData(endEffeactor);
		std::vector<AssimpNodeData*> maniplutors;

		if (manipulator == nullptr)
		{
			Logger::Err("FAILED TO SET UP IK");
			return maniplutors;
		}

		while (manipulator->name != "mixamorig_Spine2")
		{
			maniplutors.push_back(manipulator);
			manipulator = manipulator->parent;
		}

		return maniplutors;
	}

	AssimpNodeData* Animation::GetNodeData(std::string const& nodeName)
	{
		std::queue<AssimpNodeData*> boneNodes{};
		boneNodes.push(rootNode);
		AssimpNodeData* currentNode;

		while (!boneNodes.empty()) 
		{
			currentNode = boneNodes.front();
			boneNodes.pop();

			if (currentNode->name == nodeName)
			{
				return currentNode;
			}

			for (AssimpNodeData& child : currentNode->children)
			{
				boneNodes.push(&child);
			}
			
		}
		return nullptr;
	}

	void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
	{
		int size = animation->mNumChannels;

		const auto& boneInfo = model.GetBoneMap();
		int& boneCount = model.GetBoneCount(); 

		//reading channels(bones engaged in an animation and their keyframes)
		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].id = boneCount;
				boneCount++;
			}
			bones.push_back(Bone(channel->mNodeName.data,
				boneInfoMap[channel->mNodeName.data].id, channel));
		}

		boneInfoMap = boneInfo;
	}

	// Read in Heirarchy
	void Animation::ReadHeirarchyData(AssimpNodeData* dest, const aiNode* src)
	{
		GL_ASSERT(src);

		dest->name = src->mName.data;
		dest->transformation = AssimpToGLMH::ConvertMatrixToGLMFormat(src->mTransformation);
		dest->childNum = src->mNumChildren;
		
		for (unsigned int i = 0; i < src->mNumChildren; i++)
		{
			AssimpNodeData* newData = new AssimpNodeData();
			newData->parent = dest;
			ReadHeirarchyData(newData, src->mChildren[i]);
			dest->children.push_back(*newData);

			/*AssimpNodeData newData;
			newData.parent = &dest;
			ReadHeirarchyData(newData, src->mChildren[i]);
			dest.children.push_back(newData);*/
		}
	}
}
