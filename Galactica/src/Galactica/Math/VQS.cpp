#include "glpch.h"

#include "VQS.h"

#include "MathHelper.h"

namespace Galactica
{
	VQS::VQS()
		: translationVector(),
		scalingVector(1.0f)
	{
	}

	VQS::VQS(const glm::vec3& vector, const QuatFloat& quat)
		: translationVector(vector),
		quatRotation(quat),
		scalingVector(1.0f)
	{
	}

	VQS::VQS(const glm::vec3& vector, const QuatFloat& quat, float scale)
		: translationVector(vector),
		quatRotation(quat),
		scalingVector(scale)
	{
	}

	inline void VQS::SetTranslationVector(const glm::vec3& vector)
	{
		this->translationVector = vector;
	}

	inline const glm::vec3& VQS::GetTranslationVector() const
	{
		return translationVector;
	}

	glm::vec3 VQS::GetTranslationVector()
	{
		return translationVector;
	}

	inline void VQS::SetRotation(const QuatFloat& quat)
	{
		this->quatRotation = quat;
	}

	inline const QuatFloat& VQS::GetRotation() const
	{
		return quatRotation;
	}

	QuatFloat VQS::GetRotation()
	{
		return quatRotation;
	}

	void VQS::SetScaleVector(float scale)
	{
		this->scalingVector.x = scale;
		this->scalingVector.y = scale;
		this->scalingVector.z = scale;
	}

	float VQS::GetScale() const
	{
		return this->scalingVector.x;
	}

	inline const glm::vec3& VQS::GetScaleVector() const
	{
		return scalingVector;
	}

	glm::vec3 VQS::GetScaleVector()
	{
		return scalingVector;
	}

	VQS& VQS::operator+(const VQS& vqs)
	{
		this->translationVector += vqs.translationVector;
		this->quatRotation += vqs.quatRotation;
		this->scalingVector += vqs.scalingVector;
		return *this;
	}

	VQS& VQS::operator*(float value)
	{
		this->translationVector *= value;
		this->quatRotation *= value;
		this->scalingVector *= value;
		return *this;
	}

	VQS& VQS::operator*=(const VQS& vqs)
	{
		this->translationVector *= (vqs.translationVector * *this);
		this->quatRotation *= vqs.quatRotation;
		this->scalingVector *= vqs.scalingVector;
		return *this;
	}

	VQS VQS::operator*(const VQS& vqs) const
	{
		return {
			vqs.translationVector * *this,
			this->quatRotation * vqs.quatRotation,
			(this->scalingVector * vqs.scalingVector).x
		};
	}

	glm::vec3 VQS::operator*(const glm::vec3& vec) const
	{
		// Apply scaling
		glm::vec3 scaledVec = scalingVector * vec;

		// Convert scaledVec to quaternion 
		QuatFloat scaledVecQuat(0.0f, scaledVec.x, scaledVec.y, scaledVec.z);

		// Apply rotation using quaternion 
		QuatFloat rotatedVecQuat = quatRotation * scaledVecQuat * quatRotation.GetInverse(); 

		//Convert rotated quaternion back to vector 
		glm::vec3 rotatedVec(rotatedVecQuat.x, rotatedVecQuat.y, rotatedVecQuat.z);

		// Apply translation 
		glm::vec3 result = rotatedVec + translationVector;

		return result;
	}

	void VQS::MakeInverse()
	{
		const auto scaledTranslationInverse = (1.0f / this->scalingVector) * (this->translationVector * -1.0f);
		const auto scaledTranslationInverseQuat = QuatFloat(scaledTranslationInverse.x, scaledTranslationInverse.y, scaledTranslationInverse.z, 0.0f);

		const auto subTranslationVectorInverse = this->quatRotation.Inverse() * (scaledTranslationInverseQuat)*this->quatRotation;

		this->translationVector = glm::vec3(subTranslationVectorInverse.x, subTranslationVectorInverse.y, subTranslationVectorInverse.z);
		this->quatRotation.Inverse();
		this->scalingVector = 1.0f / this->scalingVector;
	}

	VQS VQS::Inverse() const
	{
		VQS inverse;

		const auto scaledInverse = (1.0f / this->scalingVector.x);
		const auto translationInverse = this->translationVector * -1.0f;

		const auto rotationVector = glm::vec3(this->quatRotation.x, this->quatRotation.y, this->quatRotation.z);
		const auto rotationVectorInverse = glm::vec3(-this->quatRotation.x, -this->quatRotation.y, -this->quatRotation.z);

		const auto subTranslationVectorInverse = rotationVectorInverse * (scaledInverse)*rotationVector;

		inverse.translationVector = subTranslationVectorInverse;
		inverse.quatRotation = this->quatRotation.GetInverse();
		inverse.scalingVector = 1.0f / this->scalingVector;

		return inverse;
	}

	VQS VQS::Interpolate(const VQS& vqsOne, const VQS& vqsTwo, float t_Translation, float t_Rotation, float t_Scale)
	{
		const auto vqsOneTranslateInternal = GLMInternalHelper::ConvertGLMVectorToInternal(vqsOne.translationVector);
		const auto vqsTwoTranslateInternal = GLMInternalHelper::ConvertGLMVectorToInternal(vqsTwo.translationVector);

		const auto translate =
			GLMInternalHelper::ConvertInternalVectorToGLM(
				Vec3f::Lerp(vqsOneTranslateInternal,vqsTwoTranslateInternal,t_Translation));

		const auto rotation =
				QuatFloat::Slerp(vqsOne.quatRotation,vqsTwo.quatRotation,t_Rotation).Normalize();

		const auto scale = std::pow(vqsTwo.scalingVector.x / vqsOne.scalingVector.x, t_Scale) * vqsOne.scalingVector.x;

		return { translate, rotation, scale };
	}

	VQS VQS::GetUnitVQS()
	{
		VQS result;

		result.translationVector = glm::vec3(0.0f);
		result.quatRotation = QuatFloat();
		result.scalingVector = glm::vec3(1.0f);

		return result;
	}
}
