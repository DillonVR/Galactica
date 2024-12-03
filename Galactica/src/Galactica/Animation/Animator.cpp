#include "glpch.h"

#include "Animator.h"

#include "Galactica/Math/MathHelper.h"

namespace Galactica
{
	//Initial setup of the animator
	void Animator::Play(Animation* pAnimation)
	{
		if (pAnimation != currentAnimation)
		{
			currentAnimation = pAnimation;
			currentTime = 0.0f;

			finalBoneMatrices.reserve(100);
			debugBone.resize(200);

			for (int i = 0; i < 100; i++)
				finalBoneMatrices.emplace_back(glm::mat4(1.0f));
		}
	}

	// Calculate each bones transformation using GLM -- We don't use use this function
	void Animator::CalcBoneTransformation(const AssimpNodeData* node, glm::mat4 parentTransform, bool ik)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		if (Bone* Bone = currentAnimation->FindBone(nodeName))
		{
			if (!ik)
			{
				Bone->Update(currentTime);
			}
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
			CalcBoneTransformation(&node->children[i], globalTransformation, ik);
	}

	void Animator::CalcBlendTranformation(float animationTime, std::vector<glm::mat4>& Transfomation,
		unsigned startAnimation, unsigned endAnimation, float blendFactor)
	{
	}

	// Calculate each bones transformation using VQS 
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

	void Animator::CalcBoneTransformationIK(const AssimpNodeData* node, VQS parentTransform, bool ik)
	{
		std::string nodeName = node->name;
		auto nodeTransform = GLMInternalHelper::ConvertGLMMatrixToVQS(node->transformation);

		if (Bone* Bone = currentAnimation->FindBone(nodeName))
		{
			if (!ik)
			{
				Bone->Update(currentTime);
			}
		
			nodeTransform = Bone->GetLocalVQS();
		}

		const auto globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = currentAnimation->GetBoneIDMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offset;
			finalBoneMatrices[index] = GLMInternalHelper::ConvertVQSToGLMMatrix(globalTransformation) * offset;

			//get bones
			debugBone[index * 2] = globalTransformation.translationVector;
			if (node->parent)
				debugBone[index * 2 + 1] = parentTransform.translationVector;
			else
				debugBone[index * 2 + 1] = globalTransformation.translationVector;
		}

		for (int i = 0; i < node->childNum; i++)
		{
			CalcBoneTransformationIK(&node->children[i], globalTransformation, ik);
		}

	}

	void Animator::SetupIK(std::string const& effector)
	{
		manipulators = currentAnimation->GetManipulators(effector);

		constraints.resize(manipulators.size(), glm::vec2{0.0f});
		angleRotated.resize(manipulators.size(), 0);

		constraints[0] = glm::vec2{-10.0f ,10.0f};
		constraints[1] = glm::vec2{-30.0f, 30.0f};
		constraints[2] = glm::vec2{0.0f, 110.0f};
		constraints[3] = glm::vec2{-90.0f, 60.0f};
		constraints[4] = glm::vec2{-1.0, 1.0};
	}

	void Animator::ResetIK()
	{
		for (unsigned int i = 0; i < 5; ++i)
		{
			angleRotated[i] = 0.0f;
		}
	}

	void Animator::SolveCCDIK(glm::vec3 const& target, float deltatime)
	{

		std::unordered_map<std::string, BoneInfo> const& bone_info_map = currentAnimation->GetBoneIDMap();

		for (unsigned int i = 0; i < manipulators.size() - 1; ++i) {
			// getting transformation matrix in end effector's local space
			glm::mat4 transform_in_ee_local_space{ 1.0f };
			for (int j = i; j >= 0; --j) {
				Bone* bone = currentAnimation->FindBone(manipulators[j]->name);
				transform_in_ee_local_space *= bone->getLocalTransform();
			}

			// vector from current joint to current position of end effector
			glm::vec3 v_ck{
				transform_in_ee_local_space[3][0],
				transform_in_ee_local_space[3][1],
				transform_in_ee_local_space[3][2],
			};

			Bone* parent_bone = currentAnimation->FindBone(manipulators[i + 1]->name);
			int parent_id = bone_info_map.at(manipulators[i + 1]->name).id;
			glm::mat4 parent_transform = finalBoneMatrices[parent_id] *
				glm::inverse(bone_info_map.at(parent_bone->GetBoneName()).offset);
			// vector from current joint to final position of end effector
			glm::vec3 v_dk = glm::inverse(parent_transform) * glm::vec4{ target, 1.0f };

			v_ck = glm::normalize(v_ck);
			v_dk = glm::normalize(v_dk);

			// axis of rotation = cross(v_ck, v_dk)
			glm::vec3 axis = glm::cross(v_ck, v_dk);
			// angle to be rotated = dot(v_ck, v_dk)
			float angle = acosf(glm::dot(v_ck, v_dk)) * deltatime;


			// clamp angle within constraints
			if (angleRotated[i] + glm::degrees(angle) < constraints[i].x ||
				angleRotated[i] + glm::degrees(angle) > constraints[i].y)
			{
				angle = 0.0f;
			}
			else 
			{
				// keep track of how much this manipulator has been rotated
				angleRotated[i] += glm::degrees(angle);
			}

			// exit condition
			if (glm::length(axis) < 0.01f) {
				break;
			}
			axis = glm::normalize(axis);

			glm::mat4 rotation_mat = glm::mat4{ glm::angleAxis(angle, axis) };
			rotation_mat *= transform_in_ee_local_space;

			// transform rotation matrix back to local space of manipulator
			for (unsigned int j = 0; j < i; ++j) {
				Bone* bone = currentAnimation->FindBone(manipulators[j]->name);
				rotation_mat *= glm::inverse(bone->getLocalTransform());
			}
			currentAnimation->FindBone(manipulators[i]->name)->setLocalTransform(rotation_mat);

			// update final bone matrix
			for (int j = i; j >= 0; --j) {
				int curr_index = bone_info_map.at(manipulators[j]->name).id;
				int parent_index = bone_info_map.at(manipulators[j + 1]->name).id;

				Bone* curr_bone = currentAnimation->FindBone(manipulators[j]->name);
				Bone* parent_bone = currentAnimation->FindBone(manipulators[j + 1]->name);
				glm::mat4 global_transform = finalBoneMatrices[parent_index]
					* glm::inverse(bone_info_map.at(parent_bone->GetBoneName()).offset);
				global_transform *= curr_bone->getLocalTransform();
				finalBoneMatrices[curr_index] = global_transform * bone_info_map.at(curr_bone->GetBoneName()).offset;
			}
		}

		CalcBoneTransformation(currentAnimation->GetRootNode(), glm::mat4{ 1.0f }, true);

		//CalcBoneTransformationIK(currentAnimation->GetRootNode(), VQS(), true);
	}

	void Animator::SolveFABRIK(glm::vec3 const& target, float deltatime)
	{

	}

	std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
	{
		return finalBoneMatrices;
	}

	//use VQS to update the Animation
	void Animator::UpdateAnimation(float time, float normTime, float speed)
	{
		//deltaTime = time;

		if (normTime < 0.2f || normTime > 0.8f)
		{
			deltaTime = time * speed;
		}
		else
			deltaTime = time * 1.2f;

		if (currentAnimation)
		{

			currentTime += currentAnimation->GetTicksPerSecond() * deltaTime;
			currentTime = fmod(currentTime, currentAnimation->GetDuration());
			bonePositions.clear();
			CalcBoneTransformation(currentAnimation->GetRootNode(), glm::mat4(1.0f), false);
			//CalcBoneTransformationIK(currentAnimation->GetRootNode(), VQS(), false);
		}
	}
}
