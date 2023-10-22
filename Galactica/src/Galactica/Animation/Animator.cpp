#include "glpch.h"

#include "Animator.h"

namespace Galactica
{
	void Animator::Play(Animation* pAnimation)
	{
		currentAnimation = pAnimation;
		currentTime = 0.0f;

		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
			finalBoneMatrices.push_back(glm::mat4(1.0f));
	}

	void Animator::CalcBoneTransformation(const AssimpNodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		Bone* Bone = currentAnimation->FindBone(nodeName);

		if (Bone)
		{
			Bone->Update(currentTime);
			nodeTransform = Bone->getLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = currentAnimation->GetBoneIDMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offset;
			finalBoneMatrices[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node->childNum; i++)
			CalcBoneTransformation(&node->children[i], globalTransformation);
	}

	std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
	{
		return finalBoneMatrices;
	}

	void Animator::UpdateAnimation(float time)
	{
		deltaTime = time;
		if (currentAnimation)
		{
			currentTime += currentAnimation->GetTicksPerSecond() * time;
			currentTime = fmod(currentTime, currentAnimation->GetDuration());
			CalcBoneTransformation(&currentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}
}
