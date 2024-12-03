#pragma once
#include "StepTimer.h"
#include "Galactica/Core/Timestep.h"
#include "Galactica/Math/Vec3.h"
#include "Galactica/Renderer/Camera.h"
#include "Galactica/Vendor/glm/glm/fwd.hpp"

constexpr float ERROR_THRESHOLD = 0.01f;
constexpr float MAX_PARAM_INTERVAL = 0.01f;

namespace Galactica
{

	struct TableEntry
	{
		float u;
		float length;
	};


	class GALACTICA_API Movement
	{
	public:
		Movement(float currentTime) :
			m_T1(0.2f), m_T2(0.8f),
			m_TravelDuration(15.0f),
			m_TravelBeginTime(currentTime)
		{
			m_V0 = 2.0f / (1.0f - m_T1 + m_T2);

			GenerateDefultPath();
			//ComputeTable();
		}

		glm::vec3 InterpolationFunc(float u,
			glm::vec3 P0, glm::vec3 P1,
			glm::vec3 P2, glm::vec3 P3);

		glm::mat4 Update(Galactica::StepTimer timer);

		std::vector<glm::vec3> m_PlotPoints;

		std::vector<glm::vec3> m_StartingPoints;

		std::vector<glm::vec3> m_ControlPoints;

		float getSpeedFactor() const { return  m_SpeedFactor; }

		float GetNormTime() const { return m_NormalizedTime; }

		float m_NormalizedTime = 0.0f; // [0, 1]

		float m_SpeedFactor; //[0, 1] & 1 = full velocity

		void GenerateDefultPath();

		void GenerateNewPath(glm::vec3 target, glm::vec3 position);

		void ComputeTable(std::vector<glm::vec3> path);

		bool path_completed;

		bool loop;

		bool reset;

		void Restart(Galactica::StepTimer timer);

	private:
		float GetDistanceFromTime(float time);

		//outIndex is the index in the final table
		void GetDistanceFromU(float u, float& outDistance, int& outIndex);

		void GetUFromDistance(float s, float& outU, int& outIndex);

		float GetVelocity(float time) const;


		std::vector<TableEntry> m_FinalTable;

		TableEntry m_LastTableEntry;

		float m_V0;

		float m_T1, m_T2;

		float m_TravelDuration; //total time it takes to travel on the path

		float m_TravelBeginTime;

		glm::mat4 translateMat;
		glm::mat4 rotation;

	};
};



