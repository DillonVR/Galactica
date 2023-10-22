#pragma once

#include "assimp/scene.h"

#include "Galactica/Vendor/glm/glm/glm.hpp"
#include "Galactica/Vendor/glm/glm/gtx/quaternion.hpp"


namespace Galactica
{
	struct KeyPosition
	{
		glm::vec3 position;
		float timeStamp;
	};

	struct KeyRotation
	{
		glm::quat orientation;
		float timeStamp;

	};

	struct KeyScale
	{
		glm::vec3 scale;
		float timeStamp;
	};

	class Bone
	{
	public:
		
		Bone(const std::string& name, int ID, const aiNodeAnim* channel);

		void Update(float animationTime);

		glm::mat4 getLocalTransform();

		std::string GetBoneName() const;

		int GetBoneID();

		int GetPosIndex(float animationTime);

		int GetRotIndex(float animationTime);

		int GetScaleIndex(float animationTime);

	private:

		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

		glm::mat4 InterpolatePos(float animationTime);

		glm::mat4 InterpolateRot(float animationTime);

		glm::mat4 InterpolateScale(float animationTime);

		std::vector<KeyPosition> positions;
		std::vector<KeyRotation> rotations;
		std::vector<KeyScale> scales;

		int numPositions;
		int numRotations;
		int numScalings;

		std::string name;
		int ID;
		glm::mat4 localTransform;

	};
}

