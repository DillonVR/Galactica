#pragma once
#include "Animation.h"
#include "Galactica/Math/VQS.h"

namespace Galactica
{
	class GALACTICA_API Animator
	{
	public:
		Animator() = default;

		void UpdateAnimation(float time);

		void Play(Animation* pAnimation);

		void CalcBoneTransformation(const AssimpNodeData* node, glm::mat4 parentTransform);

		void CalcBoneTransformationVQS(const AssimpNodeData* node, VQS parentTransform);

		std::vector<glm::mat4> GetFinalBoneMatrices();

		std::vector<glm::vec3> debugBone;

	private:

		std::vector<glm::mat4> finalBoneMatrices;

		Animation* currentAnimation;

		float currentTime;

		float deltaTime;

	};
}


