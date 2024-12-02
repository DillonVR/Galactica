#include "glpch.h"

#include "Animator.h"

#include "Galactica/Math/MathHelper.h"

namespace Galactica
{
	//Initial setup of the animator
	void Animator::Play(Animation* pAnimation)
	{
		currentAnimation = pAnimation;
		currentTime = 0.0f;
		
		finalBoneMatrices.reserve(100);
		debugBone.resize(200);

		for (int i = 0; i < 100; i++)
			finalBoneMatrices.emplace_back(glm::mat4(1.0f));
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
			
		++counter;
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

		bonePositions.clear();
		counter = 0;
		const std::unordered_map<std::string, BoneInfo>& bone_info_map = currentAnimation->GetBoneIDMap();

		for (unsigned int i = 0; i < manipulators.size() - 1; ++i)
		{ 
			// Getting transformation in end effector's local space 
			VQS transform_in_ee_local_space;
			for (int j = i; j >= 0; --j)
			{ 
				auto bone = currentAnimation->FindBone(manipulators[j]->name);
				Logger::Log("Manipulators: " + std::to_string(j) + " which is " + manipulators[j]->name);

				auto BoneVQS = bone->GetLocalVQS();
				glm::vec3 translation = BoneVQS.GetTranslationVector();
				QuatFloat rotation = BoneVQS.GetRotation();
				transform_in_ee_local_space = VQS(translation, rotation) * transform_in_ee_local_space;
			} 

			// Vector from current joint to current position of end effector
			glm::vec3 v_ck = transform_in_ee_local_space.translationVector;
			auto parent_bone = currentAnimation->FindBone(manipulators[i + 1]->name);

			VQS parentOffeset = GLMInternalHelper::ConvertGLMMatrixToVQS(bone_info_map.at(parent_bone->GetBoneName()).offset);
			// Vector from current joint to target
			auto inverseParentTransform = parentOffeset.Inverse();
			glm::vec3 v_dk = target;


			// Log initial and target positions
			Logger::Log("Initial Position (v_ck): ");
			std::cout << "x: " << v_ck.x << ", y: " << v_ck.y << ", z: " << v_ck.z << std::endl;
			Logger::Log("Target Position (v_dk): ");
			std::cout << "x: " << v_dk.x << ", y: " << v_dk.y << ", z: " << v_dk.z << std::endl;


			v_ck = glm::normalize(v_ck);
			v_dk = glm::normalize(v_dk);

			// Log normalized vectors
			Logger::Log("Normalized v_ck: ");
			std::cout << "x: " << v_ck.x << ", y: " << v_ck.y << ", z: " << v_ck.z << std::endl;
			Logger::Log("Normalized v_dk: ");
			std::cout << "x: " << v_dk.x << ", y: " << v_dk.y << ", z: " << v_dk.z << std::endl;

			// Axis of rotation = cross(v_ck, v_dk) 
			glm::vec3 axis = glm::cross(v_ck, v_dk);
			Logger::Log("axis: ");
			std::cout << "x: " << axis.x << ", y: " << axis.y << ", z: " << axis.z << std::endl;

			// Angle to be rotated = dot(v_ck, v_dk) 
			float angle = acosf(glm::dot(v_ck, v_dk)) * deltatime; 
			Logger::Log("Angle: " + std::to_string(angle));
			
			// Clamp angle within constraints 
			if (angleRotated[i] + glm::degrees(angle) < constraints[i].x || angleRotated[i] + glm::degrees(angle) > constraints[i].y)
			{
				angle = 0.0f;
			}
			else 
			{
				// Keep track of how much this manipulator has been rotated
				angleRotated[i] += glm::degrees(angle);
			}

			// Log clamped angle
			Logger::Log("Clamped Angle: " + std::to_string(angle));
			 
			// Exit condition 
			if (glm::length(axis) < 0.01f)
			{
				break;
			}
			axis = glm::normalize(axis);

			glm::quat glmQuat = angleAxis(angle, axis);
			QuatFloat rotation_quat = QuatFloat(glmQuat);

			VQS rotation_vqs = VQS(glm::vec3(0.0f), rotation_quat) * transform_in_ee_local_space;


			/*for (unsigned int j = 0; j < i; ++j) 
			{ 
				auto bone = currentAnimation->FindBone(manipulators[j]->name);
				auto boneVQS = bone->GetLocalVQS();
				glm::vec3 translation = boneVQS.GetTranslationVector();
				QuatFloat rotation = boneVQS.GetRotation();
				
				rotation_vqs = rotation_vqs * VQS(translation, rotation);
			}*/

			currentAnimation->FindBone(manipulators[i]->name)->SetLocalVQS(rotation_vqs);

			// Update final bone matrix 
			for (int j = i; j >= 0; --j)
			{
			
				int currentIndex = bone_info_map.at(manipulators[j]->name).id; 
				int parentIndex = bone_info_map.at(manipulators[j + 1]->name).id;
				Bone* currentBone = currentAnimation->FindBone(manipulators[j]->name);
				Bone* parentBone = currentAnimation->FindBone(manipulators[j + 1]->name);

				// Convert finalBoneMatrices and offsets to VQS using GLMInternalHelper
				VQS parentVQS = GLMInternalHelper::ConvertGLMMatrixToVQS(finalBoneMatrices[parentIndex]);
				VQS offsetVQS = GLMInternalHelper::ConvertGLMMatrixToVQS(bone_info_map.at(parentBone->GetBoneName()).offset);

				// Reset or normalize scaling vector 
				parentVQS.scalingVector = glm::vec3(1.0f); 
				offsetVQS.scalingVector = glm::vec3(1.0f);
				Logger::Log("Bone Transformation for " + manipulators[j]->name + ": ");

				// Perform the transformation
				VQS globalVQS = parentVQS * offsetVQS; 

				// Get current bone transform in VQS 
				VQS currentVQS = currentBone->GetLocalVQS();
				currentVQS.scalingVector = glm::vec3(1.0f); // Reset or normalize scaling vector

				globalVQS = globalVQS * currentVQS; 

				// Convert global_vqs back to matrix using GLMInternalHelper 
				glm::mat4 globalTransform = GLMInternalHelper::ConvertVQSToGLMMatrix(globalVQS);
				finalBoneMatrices[currentIndex] = globalTransform * bone_info_map.at(currentBone->GetBoneName()).offset;

				// Log bone transformations
				Logger::Log("Bone Transformation for " + manipulators[j]->name + ": ");
				std::cout << glm::to_string(globalTransform) << std::endl;

				int x = 0;
			} 
		}

		CalcBoneTransformationIK(currentAnimation->GetRootNode(), VQS(), true);
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
			//CalcBoneTransformation(currentAnimation->GetRootNode(), glm::mat4(1.0f), false);
			CalcBoneTransformationIK(currentAnimation->GetRootNode(), VQS(), false);
		}
	}
}
