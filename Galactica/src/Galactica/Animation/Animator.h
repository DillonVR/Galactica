#pragma once
#include "Animation.h"

namespace Galactica
{
	class GALACTICA_API Animator
	{
	public:
		Animator() = default;

		void UpdateAnimation(float time);

		void Play(Animation* pAnimation);

		void CalcBoneTransformation(const AssimpNodeData* node, glm::mat4 parentTransform);

		std::vector<glm::mat4> GetFinalBoneMatrices();

	private:

		std::vector<glm::mat4> finalBoneMatrices;

		Animation* currentAnimation;

		float currentTime;

		float deltaTime;

	};
}


