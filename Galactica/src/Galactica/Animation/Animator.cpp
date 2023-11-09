#include "glpch.h"

#include "Animator.h"

#include "Galactica/Math/MathHelper.h"

namespace Galactica
{
	void Animator::Play(Animation* pAnimation)
	{
		currentAnimation = pAnimation;
		currentTime = 0.0f;
		
		finalBoneMatrices.reserve(100);
		debugBone.resize(200);

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

			//get bones
			debugBone[index * 2] = globalTransformation[3];
			if(node->parent)
				debugBone[index * 2 + 1] = parentTransform[3];
			else
				debugBone[index * 2 + 1] = globalTransformation[3];
		}

		for (int i = 0; i < node->childNum; i++)
			CalcBoneTransformation(&node->children[i], globalTransformation);
	}

	void Animator::CalcBoneTransformationVQS(const AssimpNodeData* node, VQS parentTransform)
	{
		std::string nodeName = node->name;
		auto nodeTransform = GLMInternalHelper::ConvertGLMMatrixToVQS(node->transformation);

		if (Bone* Bone = currentAnimation->FindBone(nodeName))
		{
			Bone->Update(currentTime);
			nodeTransform = Bone->GetLocalVQS();
		}

		const auto globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = currentAnimation->GetBoneIDMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offset;
			finalBoneMatrices[index] =GLMInternalHelper::ConvertVQSToGLMMatrix(globalTransformation) * offset;

			//get bones
			debugBone[index * 2] = globalTransformation.translationVector;
			if (node->parent)
				debugBone[index * 2 + 1] = parentTransform.translationVector;
			else
				debugBone[index * 2 + 1] = globalTransformation.translationVector;
		}

		for (int i = 0; i < node->childNum; i++)
			CalcBoneTransformationVQS(&node->children[i], globalTransformation);
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
			//CalcBoneTransformation(&currentAnimation->GetRootNode(), glm::mat4(1.0f));
			CalcBoneTransformationVQS(&currentAnimation->GetRootNode(), VQS());
		}
	}
}
