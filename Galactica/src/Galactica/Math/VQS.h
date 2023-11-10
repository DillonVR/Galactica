#pragma once

#include "Quaternion.h"
#include "Galactica/Vendor/glm/glm/glm.hpp"
#include <Galactica/Vendor/glm/glm/gtx/string_cast.hpp>

namespace Galactica
{
	class VQS
	{
	public:
		VQS();

		VQS(const glm::vec3& vector, const QuatFloat& quat);

		VQS(const glm::vec3& vector, const QuatFloat& quat, float scale);

		void SetTranslationVector(const glm::vec3& vector);

		const glm::vec3& GetTranslationVector() const;

		glm::vec3 GetTranslationVector();

		void SetRotation(const QuatFloat& quat);

		const QuatFloat& GetRotation() const;

		QuatFloat GetRotation();

		void SetScaleVector(float scale);

		float GetScale() const;

		const glm::vec3& GetScaleVector() const;

		glm::vec3 GetScaleVector();

		void MakeInverse();

		VQS Inverse() const;

		static VQS Interpolate(const VQS& vqsOne, const VQS& vqsTwo, float t_Translation, float t_Rotation, float t_Scale);

		static VQS GetUnitVQS();

		VQS& operator+(const VQS& vqs);
		VQS& operator*(float value);
		VQS& operator*=(const VQS& vqs);
		VQS operator*(const VQS& vqs) const;

		friend VQS operator*(const VQS& vqs, float value)
		{
			VQS result;

			result.translationVector = vqs.translationVector * value;
			result.scalingVector = vqs.scalingVector * value;
			result.quatRotation = vqs.quatRotation * value;

			return result;
		}

		friend VQS operator*(float value, const VQS& vqs)
		{
			return {
				vqs.translationVector * value,
				vqs.quatRotation * value,
				(vqs.scalingVector * value).x
			};
		}

		friend glm::vec3 operator*(const glm::vec3& point, const VQS& vqs)
		{
			const auto scaledPoint = vqs.scalingVector * point;
			const auto scaledPointQuat = QuatFloat(scaledPoint.x, scaledPoint.y, scaledPoint.z, 0.0f);
			QuatFloat rotationInverse = vqs.quatRotation.GetInverse();

			const auto subResult = (vqs.quatRotation * (scaledPointQuat)* rotationInverse + QuatFloat(vqs.translationVector.x, vqs.translationVector.y, vqs.translationVector.z, 0.0f));

			return { subResult.x, subResult.y, subResult.z };
		}

		glm::vec3 translationVector;

		QuatFloat quatRotation;

		glm::vec3 scalingVector;
	};
};


