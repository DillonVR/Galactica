#pragma once
#include "Animation.h"
#include "Movement.h"
#include "Galactica/Math/VQS.h"

namespace Galactica
{
	class GALACTICA_API Animator
	{
	public:
		Animator() = default;

		void UpdateAnimation(float time, float normTime, float speed);

		void Play(Animation* pAnimation);

		void CalcBoneTransformation(const AssimpNodeData* node, glm::mat4 parentTransform);

		void CalcBlendTranformation(float animationTime, std::vector<glm::mat4>& Transfomation, unsigned int startAnimation, unsigned int endAnimation, float blendFactor);

		void CalcBoneTransformationVQS(const AssimpNodeData* node, VQS parentTransform);

		std::vector<glm::mat4> GetFinalBoneMatrices();

		std::vector<glm::vec3> debugBone;

	private:

		std::vector<glm::mat4> finalBoneMatrices;

		Animation* currentAnimation;

		Movement* path;

		float currentTime;

		float deltaTime;

	};
}


