#pragma once

#include "Movement.h"
#include "assimp/scene.h"
#include "Galactica/Math/VQS.h"

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
		QuatFloat orientation;
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

		glm::mat4 getLocalTransform() const;

		void setLocalTransform(const glm::mat4& newTransform);

		const VQS& GetLocalVQS() const;

		void SetLocalVQS(const VQS& newVQS);

		std::string GetBoneName() const;

		int GetBoneID() const;

		int GetPosIndex(float animationTime) const;

		int GetRotIndex(float animationTime) const;

		int GetScaleIndex(float animationTime) const;

	private:
		static float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

		// glm::mat4 InterpolatePos(float animationTime);
		//
		// glm::mat4 InterpolateRot(float animationTime);
		//
		// glm::mat4 InterpolateScale(float animationTime);

		glm::mat4 InterpolateVQSPos(float animationTime) const;
		
		glm::mat4 InterpolateVQSRot(float animationTime) const;
		
		glm::mat4 InterpolateVQSScale(float animationTime) const;

		VQS InterpolateWithVQS(float animationTime) const;

		std::vector<KeyPosition> positions;

		std::vector<KeyRotation> rotations;

		std::vector<KeyScale> scales;

		int numPositions;

		int numRotations;

		int numScalings;

		std::string name;

		int ID;

		glm::mat4 localTransform;

		VQS localVQS;

		Movement move;

	};
}

