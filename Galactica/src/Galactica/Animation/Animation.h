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

	};
	class GALACTICA_API Animation
	{
	public:
		Animation() = default;

		void LoadAnimation(const std::string& animationPath, Model* model);

		~Animation();

		Bone* FindBone(const std::string& name);

		inline float GetTicksPerSecond() { return ticksPerSecond; }

		inline float GetDuration() { return duration; }

		inline const AssimpNodeData& GetRootNode() { return rootNode; }

		inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
		{
			return boneInfoMap;
		}

	private:

		void ReadMissingBones(const aiAnimation* animation, Model& model);

		void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

		float duration;

		int ticksPerSecond;

		std::vector<Bone> bones;

		AssimpNodeData rootNode;

		std::map<std::string, BoneInfo> boneInfoMap;

	};

}

