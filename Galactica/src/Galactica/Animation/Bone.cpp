#include "glpch.h"

#include "Bone.h"
#include "AssimpToGLM.h"
#include "Movement.h"
#include "assimp/vector3.h"
#include "Galactica/Math/MathHelper.h"

#include "Galactica/Vendor/glm/glm/gtx/transform.hpp"

namespace Galactica
{
	Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
		: name(name),
		ID(ID),
		localTransform(1.0f),
		move(0)
	{
		numPositions = channel->mNumPositionKeys;

		for (int positionIndex = 0; positionIndex < numPositions; ++positionIndex)
		{
			aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
			float timeStamp = channel->mPositionKeys[positionIndex].mTime;

			KeyPosition data;

			data.position = AssimpToGLMH::GetGLMVec(aiPosition);
			data.timeStamp = timeStamp;
			positions.push_back(data);
		}

		numRotations = channel->mNumRotationKeys;
		for (int rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
		{
			aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
			float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
			KeyRotation data;
			data.orientation = AssimpInternalMathHelper::ConvertQuaternionToInternal(aiOrientation);
			data.timeStamp = timeStamp;
			rotations.push_back(data);
		}

		numScalings = channel->mNumScalingKeys;
		for (int keyIndex = 0; keyIndex < numScalings; ++keyIndex)
		{
			aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
			float timeStamp = channel->mScalingKeys[keyIndex].mTime;
			KeyScale data;
			data.scale = AssimpToGLMH::GetGLMVec(scale);
			data.timeStamp = timeStamp;
			scales.push_back(data);
		}
	}

	//Update the bone with VQS
	void Bone::Update(float animationTime)
	{
		glm::mat4 translation = InterpolateVQSPos(animationTime);
		glm::mat4 rotation = InterpolateVQSRot(animationTime);
		glm::mat4 scale = InterpolateVQSScale(animationTime);
		
		auto result = translation * rotation * scale;


		localTransform = result;// localVQS = GLMInternalHelper::ConvertGLMMatrixToVQS(result);
	}

	glm::mat4 Bone::getLocalTransform() const
	{
		return localTransform;
	}

	void Bone::setLocalTransform(const glm::mat4& newTransform)
	{
		localTransform = newTransform;
	}

	void Bone::SetLocalVQS(const VQS& newVQS)
	{
		localVQS = newVQS;
	}

	const VQS& Bone::GetLocalVQS() const
	{
		return localVQS;
	}

	std::string Bone::GetBoneName() const
	{
		return name;
	}

	int Bone::GetBoneID() const
	{
		return ID;
	}

	int Bone::GetPosIndex(float animationTime) const
	{
		for (int index = 0; index < numPositions - 1; ++index)
		{
			if (animationTime < positions[index + 1].timeStamp)
				return index;
		}
		return 0;
	}

	int Bone::GetRotIndex(float animationTime) const
	{
		for (int index = 0; index < numRotations - 1; ++index)
		{
			if (animationTime < rotations[index + 1].timeStamp)
				return index;
		}
		return 0;
	}

	int Bone::GetScaleIndex(float animationTime) const
	{
		for (int index = 0; index < numScalings - 1; ++index)
		{
			if (animationTime < scales[index + 1].timeStamp)
				return index;
		}
		return 0;
	}

	float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		const float midWay = animationTime - lastTimeStamp;
		const float diff = nextTimeStamp - lastTimeStamp;

		const float scaleFactor = midWay / diff;
		return scaleFactor;
	}
	// Translation of bones with vector (Lerp)
	glm::mat4 Bone::InterpolateVQSPos(float animationTime) const
	{
		const int firstPositionIndex = GetPosIndex(animationTime);
		const int secondPositionIndex = firstPositionIndex + 1;

		const auto positionScaleFactor = GetScaleFactor(
			positions[firstPositionIndex].timeStamp,
			positions[secondPositionIndex].timeStamp,
			animationTime);

		if (numPositions == 1)
		{
			return translate(glm::mat4(1.0f), positions[0].position);
		}
		else
		{
			const auto positionOne = GLMInternalHelper::ConvertGLMVectorToInternal(positions[firstPositionIndex].position);
			const auto positionTwo = GLMInternalHelper::ConvertGLMVectorToInternal(positions[secondPositionIndex].position);
			//Lerp
			const auto finalPosition = GLMInternalHelper::ConvertInternalVectorToGLM(
				Vec3f::Lerp(positionOne,positionTwo,positionScaleFactor));

			return translate(glm::mat4(1.0f), finalPosition);
		}
	}
	// Roatation of bones via VQS with  SLerp
	glm::mat4 Bone::InterpolateVQSRot(float animationTime) const
	{
		const int firstRotationIndex = GetRotIndex(animationTime);
		const int secondRotationIndex = firstRotationIndex + 1;

		const auto rotationScaleFactor = GetScaleFactor(
			rotations[firstRotationIndex].timeStamp,
			rotations[secondRotationIndex].timeStamp,
			animationTime);

		if (numRotations == 1)
		{
			const auto rotation = QuatFloat::Normalize(rotations[0].orientation);
			return GLMInternalHelper::ConvertQuaternionToGLMMatrix(rotation);
		}
		else
		{
			QuatFloat first = rotations[firstRotationIndex].orientation;
			first = first.Normalize();
			QuatFloat last = rotations[secondRotationIndex].orientation;
			last = last.Normalize();
			//SLerp
			QuatFloat finalRotation = QuatFloat::Slerp(
				first, last, rotationScaleFactor);
			finalRotation = (finalRotation).Normalize();

			return GLMInternalHelper::ConvertQuaternionToGLMMatrix(finalRotation);
		}
	}
	// Scaling of bones
	glm::mat4 Bone::InterpolateVQSScale(float animationTime) const
	{
		const int scaleIndex = GetScaleIndex(animationTime);
		const int secondScaleIndex = scaleIndex + 1;
		glm::vec3 finalScale;

		if (numScalings == 1)
		{
			return scale(glm::mat4(1.0f), scales[0].scale);
		}
		else
		{
			float scaleFactor = GetScaleFactor(scales[scaleIndex].timeStamp, scales[secondScaleIndex].timeStamp, animationTime);

			finalScale = mix(scales[scaleIndex].scale, scales[secondScaleIndex].scale, scaleFactor);
		}

			
		return glm::scale(glm::mat4(1.0f), finalScale);
	}

	//GLM POSITION, SCALING, ROTATION WITH GLM

	// glm::mat4 Bone::InterpolatePos(float animationTime)
	// {
	// 	if (1 == numPositions)
	// 		return glm::translate(glm::mat4(1.0f), positions[0].position);
	//
	// 	int p0Index = GetPosIndex(animationTime);
	// 	int p1Index = p0Index + 1;
	//
	// 	float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp,positions[p1Index].timeStamp, animationTime);
	//
	// 	glm::vec3 finalPosition = glm::mix(positions[p0Index].position,positions[p1Index].position, scaleFactor);
	//
	// 	return glm::translate(glm::mat4(1.0f), finalPosition);
	// }
	//
	// glm::mat4 Bone::InterpolateRot(float animationTime)
	// {
	// 	if (1 == numRotations)
	// 	{
	// 		auto rotation = glm::normalize(rotations[0].orientation);
	// 		return glm::toMat4(rotation);
	// 	}
	//
	// 	int p0Index = GetRotIndex(animationTime);
	// 	int p1Index = p0Index + 1;
	//
	// 	float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp,rotations[p1Index].timeStamp, animationTime);
	//
	// 	glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation,rotations[p1Index].orientation, scaleFactor);
	//
	// 	finalRotation = glm::normalize(finalRotation);
	//
	// 	return glm::toMat4(finalRotation);
	// }
	//
	// glm::mat4 Bone::InterpolateScale(float animationTime)
	// {
	// 	if (1 == numScalings)
	// 		return glm::scale(glm::mat4(1.0f), scales[0].scale);
	//
	// 	int p0Index = GetScaleIndex(animationTime);
	// 	int p1Index = p0Index + 1;
	//
	// 	float scaleFactor = GetScaleFactor(scales[p0Index].timeStamp,scales[p1Index].timeStamp, animationTime);
	//
	// 	glm::vec3 finalScale = glm::mix(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);
	//
	// 	return glm::scale(glm::mat4(1.0f), finalScale);
	// }

}
