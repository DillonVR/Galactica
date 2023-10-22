#include "glpch.h"

#include "Bone.h"
#include "AssimpToGLM.h"
#include "assimp/vector3.h"

#include "Galactica/Vendor/glm/glm/gtx/transform.hpp"

namespace Galactica
{
	Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
		:	name(name),
			ID(ID),
			localTransform(1.0f)
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
			data.orientation = AssimpToGLMH::GetGLMQuat(aiOrientation);
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

	void Bone::Update(float animationTime)
	{
		glm::mat4 translation = InterpolatePos(animationTime);
		glm::mat4 rotation = InterpolateRot(animationTime);
		glm::mat4 scale = InterpolateScale(animationTime);
		localTransform = translation * rotation * scale;
	}

	glm::mat4 Bone::getLocalTransform()
	{
		return localTransform;
	}

	std::string Bone::GetBoneName() const
	{
		return name;
	}

	int Bone::GetBoneID()
	{
		return ID;
	}

	int Bone::GetPosIndex(float animationTime)
	{
		for (int index = 0; index < numPositions - 1; ++index)
		{
			if (animationTime < positions[index + 1].timeStamp)
				return index;
		}
	}

	int Bone::GetRotIndex(float animationTime)
	{
		for (int index = 0; index < numRotations - 1; ++index)
		{
			if (animationTime < rotations[index + 1].timeStamp)
				return index;
		}
	}

	int Bone::GetScaleIndex(float animationTime)
	{
		for (int index = 0; index < numScalings - 1; ++index)
		{
			if (animationTime < scales[index + 1].timeStamp)
				return index;
		}
	}

	float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		float scaleFactor = 0.0f;
		float midWay = animationTime - lastTimeStamp;
		float diff = nextTimeStamp - lastTimeStamp;

		scaleFactor = midWay / diff;
		return scaleFactor;
	}

	glm::mat4 Bone::InterpolatePos(float animationTime)
	{
		if (1 == numPositions)
			return glm::translate(glm::mat4(1.0f), positions[0].position);

		int p0Index = GetPosIndex(animationTime);
		int p1Index = p0Index + 1;

		float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp,positions[p1Index].timeStamp, animationTime);

		glm::vec3 finalPosition = glm::mix(positions[p0Index].position,positions[p1Index].position, scaleFactor);

		return glm::translate(glm::mat4(1.0f), finalPosition);
	}

	glm::mat4 Bone::InterpolateRot(float animationTime)
	{
		if (1 == numRotations)
		{
			auto rotation = glm::normalize(rotations[0].orientation);
			return glm::toMat4(rotation);
		}

		int p0Index = GetRotIndex(animationTime);
		int p1Index = p0Index + 1;

		float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp,rotations[p1Index].timeStamp, animationTime);

		glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation,rotations[p1Index].orientation, scaleFactor);

		finalRotation = glm::normalize(finalRotation);

		return glm::toMat4(finalRotation);
	}

	glm::mat4 Bone::InterpolateScale(float animationTime)
	{
		if (1 == numScalings)
			return glm::scale(glm::mat4(1.0f), scales[0].scale);

		int p0Index = GetScaleIndex(animationTime);
		int p1Index = p0Index + 1;

		float scaleFactor = GetScaleFactor(scales[p0Index].timeStamp,scales[p1Index].timeStamp, animationTime);

		glm::vec3 finalScale = glm::mix(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);

		return glm::scale(glm::mat4(1.0f), finalScale);
	}
}
