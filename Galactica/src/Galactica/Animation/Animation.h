#pragma once

#include "Bone.h"

#include "Galactica/Renderer/Model.h"
#include "Galactica/Vendor/glm/glm/glm.hpp"


namespace Galactica
{
	struct AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childNum;
		std::vector<AssimpNodeData> children;
		AssimpNodeData* parent;

	};

	class GALACTICA_API Animation
	{
	public:
		Animation() = default;

		void LoadAnimation(const std::string& animationPath, Model* model);

		~Animation();

		Bone* FindBone(const std::string& name);

		float GetTicksPerSecond() { return ticksPerSecond; }

		float GetDuration() { return duration; }

		const AssimpNodeData& GetRootNode() { return rootNode; }

		const std::unordered_map<std::string, BoneInfo>& GetBoneIDMap()
		{
			return boneInfoMap;
		}

		std::vector<AssimpNodeData*> GetManipulators(std::string const& endEffeactor);

		AssimpNodeData* GetNodeData(std::string const& nodeName);

	private:

		void ReadMissingBones(const aiAnimation* animation, Model& model);

		void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

		float duration;

		int ticksPerSecond;

		std::vector<Bone> bones;

		AssimpNodeData rootNode;

		std::unordered_map<std::string, BoneInfo> boneInfoMap;

	};

}

