#pragma once
#include "Quaternion.h"
#include "VQS.h"
#include "assimp/quaternion.h"
#include "assimp/vector3.h"
#include "Galactica/Vendor/glm/glm/glm.hpp"
#include "Galactica/Vendor/glm/glm/gtc/quaternion.hpp"

namespace Galactica
{
	class GLMInternalHelper
	{
	public:
		static glm::mat4 ConvertQuaternionToGLMMatrix(const QuatFloat& quaternion)
		{
			const auto right = quaternion * Vec3f(1.0f, 0, 0);
			const auto up = quaternion * Vec3f(0, 1.0f, 0);
			const auto forward = quaternion * Vec3f(0, 0, 1.0f);

			return {
				right.x,			right.y,			right.z,			0,
				up.x,			up.y,			up.z,			0,
				forward.x,		forward.y,		forward.z,		0,
				0,				0,				0,				1
			};
		}

		static QuatFloat ConvertGLMMatrixToQuaternion(const glm::mat4& matrix)
		{
			auto rotationMat = glm::mat3(matrix);
			for (int i = 0; i < 3; i++)
				rotationMat[i] = glm::normalize(rotationMat[i]);

			const auto glmQuat = glm::quat_cast(rotationMat);

			return { glmQuat.x, glmQuat.y, glmQuat.z, glmQuat.w };
		}

		static Vec3f ConvertGLMVectorToInternal(const glm::vec3& vector)
		{
			return { vector.x, vector.y, vector.z };
		}

		static Vec4f ConvertGLMVectorToInternal(const glm::vec4& vector)
		{
			return { vector.x, vector.y, vector.z, vector.w };
		}

		static glm::vec3 ConvertInternalVectorToGLM(const Vec3f& vector)
		{
			return { vector.x, vector.y, vector.z };
		}

		static glm::vec4 ConvertInternalVectorToGLM(const Vec4f& vector)
		{
			return { vector.x, vector.y, vector.z, vector.w };
		}

		static VQS ConvertGLMMatrixToVQS(const glm::mat4& matrix)
		{
			return {
				glm::vec3(matrix[3]),
				ConvertGLMMatrixToQuaternion(matrix),
				glm::length(glm::vec3(matrix[0]))
			};
		}

		static glm::mat4 ConvertVQSToGLMMatrix(const VQS& vqs)
		{
			const auto translation = glm::translate(glm::mat4(1.0f), vqs.translationVector);

			const auto right = vqs.quatRotation * Vec3f(1, 0, 0);
			const auto up = vqs.quatRotation * Vec3f(0, 1, 0);
			const auto forward = vqs.quatRotation * Vec3f(0, 0, 1);
			const glm::mat4 rotation = {
					right.x,		right.y,		right.z,		0,
					up.x,			up.y,			up.z,			0,
					forward.x,		forward.y,		forward.z,		0,
					0,				0,				0,				1
			};

			const auto scale = glm::scale(glm::mat4(1.0f), vqs.scalingVector);

			return translation * rotation * scale;
		}
	};

	class AssimpInternalMathHelper
	{
	public:
		static Vec3f ConvertVectorToInternal(const aiVector3D& vector)
		{
			return { vector.x, vector.y, vector.z };
		}

		static QuatFloat ConvertQuaternionToInternal(const aiQuaternion& orientation)
		{
			return {
				static_cast<float>(orientation.x),
				static_cast<float>(orientation.y),
				static_cast<float>(orientation.z),
				static_cast<float>(orientation.w) };
		}
	};
}
