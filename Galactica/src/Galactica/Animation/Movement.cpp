#include "glpch.h"
#include "Movement.h"

#include <iomanip>
#include <map>
#include <list>

#include "imgui.h"
#include "StepTimer.h"
#include "Galactica/Math/Vec4.h"
#include "Galactica/Vendor/glm/glm/ext/scalar_constants.hpp"
#include "Galactica/Vendor/glm/glm/gtx/compatibility.hpp"
#include "Galactica/Vendor/glm/glm/gtx/exterior_product.hpp"

namespace Galactica
{
	glm::vec3 Movement::InterpolationFunc(float u, glm::vec3 P0, glm::vec3 P1, glm::vec3 P2, glm::vec3 P3)
	{
		float u2 = u * u;
		float u3 = u2 * u;

		float b0 = -u3 + 3 * u2 - 3 * u + 1;
		float b1 = 3 * u3 - 6 * u2 + 3 * u;
		float b2 = -3 * u3 + 3 * u2;
		float b3 = u3;

		return b0 * P0 + b1 * P1 + b2 * P2 + b3 * P3;
	}

	float Movement::DistanceXZ(const glm::vec3& vec1, const glm::vec3& vec2)
	{
		float dx = vec2.x - vec1.x;
		float dz = vec2.z - vec1.z;
		return std::sqrt(dx * dx + dz * dz);
	}

	void Movement::GenerateNewPath(glm::vec3 target, glm::vec3 position)
	{
		m_newPoints.clear();

		target.y = 0.0f;
		glm::vec3 dir = target - position;
		dir = normalize(dir);

		float totalDistance = DistanceXZ(position, target);
		// Print distance
		std::cout << "Total Distance: " << totalDistance << '\n';
		float segmentsDistance = totalDistance / 8.0f;

		m_newPoints.emplace_back(glm::vec3{ position + (dir * (-segmentsDistance)) });

		for (unsigned int i = 0; i <= 8; ++i)
		{
			m_newPoints.emplace_back(glm::vec3{ position + (dir * (i * segmentsDistance)) });
		}

		std::cout << "New Points:" << std::endl;
		for (const auto& point : m_newPoints)
		{
			std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
		}

		ComputeTable(m_newPoints);
	}

	glm::mat4 Movement::Update(Galactica::StepTimer timer)
	{
		
		if (reset)
		{
			Restart(timer);
			reset = false;
		}

		float velocity = GetVelocity(m_NormalizedTime);
		m_SpeedFactor = velocity / m_V0;

		//t in [0, 1]
		m_NormalizedTime = (timer.GetTotalSeconds() - m_TravelBeginTime) / m_TravelDuration;

		//std::cout << "Normalized Time after: " << m_NormalizedTime << '\n';


		if (m_NormalizedTime < 1.0f)
		{
			path_completed = false;

			//for sliding/skidding
			

			//get distance based on t
			float dist = GetDistanceFromTime(m_NormalizedTime);

			//get u based on distance
			float u;
			int entryIndex;
			GetUFromDistance(dist, u, entryIndex);

			// convert u from [0,1] to [0, n] where n = number of segments
			u *= m_LastTableEntry.u;

			auto entriesPerSegment = m_FinalTable.size() / (m_newPoints.size() - 3);

			// convert u to [0, 1] for this particular segment
			u -= (entryIndex / entriesPerSegment);

			// find the index for control points in this segment
			int index = (entryIndex / entriesPerSegment) * 3 + 1;
			auto P0 = m_ControlPoints[index];
			auto P1 = m_ControlPoints[index + 1];
			auto P2 = m_ControlPoints[index + 2];
			auto P3 = m_ControlPoints[index + 3];

			auto position = InterpolationFunc(u, P0, P1, P2, P3);

			// Print control points and position
			/*std::cout << "P0: (" << P0.x << ", " << P0.y << ", " << P0.z << ")\n";
			std::cout << "P1: (" << P1.x << ", " << P1.y << ", " << P1.z << ")\n";
			std::cout << "P2: (" << P2.x << ", " << P2.y << ", " << P2.z << ")\n";
			std::cout << "P3: (" << P3.x << ", " << P3.y << ", " << P3.z << ")\n";
			std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";*/


			//Calculate orientation 
			auto deltaU = m_FinalTable[1].u - m_FinalTable[0].u;
			auto W = InterpolationFunc(u + deltaU, P0, P1, P2, P3) - InterpolationFunc(u, P0, P1, P2, P3);
			auto NW = normalize(W);

			glm::vec3 temp = glm::vec3(0, 1, 0);
			auto U = cross(temp, NW);
			auto V = cross(NW, U);

		    float pi = 2 * acos(0);

			rotation = glm::mat4(
				U.x, U.y, U.z, 0,
				V.x, V.y, V.z, 0,
				NW.x, NW.y, NW.z, 0,
				0, 0, 0, 1) * glm::mat4(pi);

			// Print rotation matrix
			/*std::cout << "\n\nRotation Matrix:" << '\n';
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					std::cout << std::setw(10) << std::fixed << std::setprecision(4) << rotation[i][j] << " ";
				}
				std::cout << '\n';
			}*/

			// Loop
			if (m_NormalizedTime > 1.0f && loop)
			{
				Restart(timer);
			}

			translateMat = translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));

			//// Print translateMat matrix
			//std::cout << "\n\nTranslate Matrix:" << '\n';
			//for (int i = 0; i < 4; ++i)
			//{
			//	for (int j = 0; j < 4; ++j)
			//	{
			//		std::cout << std::setw(10) << std::fixed << std::setprecision(4) << translateMat[i][j] << " ";
			//	}
			//	std::cout << '\n';
			//}
			//int i = 0;
		}
		else
		{
			path_completed = true;
		}

		return translateMat * rotation;
	}

	void Movement::Restart(Galactica::StepTimer timer)
	{
		m_TravelBeginTime = static_cast<float>(timer.GetTotalSeconds());
	}

	float Movement::GetDistanceFromTime(float time)
	{
		if (0.0f < time && time < m_T1)
		{
			return (m_V0 / (2 * m_T1)) * powf(time, 2);
		}
		if (m_T1 < time && time < m_T2)
		{
			return m_V0 * (time - (m_T1 / 2.0f));
		}
		if (m_T2 < time && time < 1.0f)
		{
			return (((m_V0 * (time - m_T2)) / (2.0f * (1 - m_T2))) * (2 - time - m_T2)) + (m_V0 * (m_T2 - (m_T1 / 2.0f)));
		}
	}

	void Movement::GetDistanceFromU(float u, float& outDistance, int& outIndex)
	{
		for (int i = 1; i < m_FinalTable.size(); ++i)
		{
			if (m_FinalTable[i].u > u)
			{
				float lerpFactor = (u - m_FinalTable[i - 1].u) / (m_FinalTable[i].u - m_FinalTable[i - 1].u);

				outIndex = i;
				outDistance = glm::lerp(m_FinalTable[i - 1].length, m_FinalTable[i].length, lerpFactor);
				return;
			}
		}
	}

	void Movement::GetUFromDistance(float s, float& outU, int& outIndex)
	{
		float uMin = 0.0f;
		float uMax = 1.0f;

		float length;
		float u;
		int entryIndex;

		 do
		 {
		 	u = (uMin + uMax) / 2.0f;
		 	GetDistanceFromU(u, length, entryIndex);
		
		 	if (s > length)
		 		uMin = u;
		 	else
		 		uMax = u;
		
		 } while (fabsf(s - length) > 0.0001f);

		outU = u;
		outIndex = entryIndex;
	}

	float Movement::GetVelocity(float time) const
	{
		if (0 < time && time < m_T1)
			return m_V0 * time / m_T1;

		else if (m_T1 < time && time < m_T2)
			return m_V0;

		else if (m_T2 < time && time < 1.0f)
			return m_V0 * (1.0f - time) / (1.0f - m_T2);
		
	}

	void Movement::GenerateDefultPath()
	{
		
		m_StartingPoints.clear();

		m_StartingPoints.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
		m_StartingPoints.push_back(glm::vec3(0.1f, 0.1f, 0.0f));
		m_StartingPoints.push_back(glm::vec3(-7.0f, 0.1f, -7.0f));
		m_StartingPoints.push_back(glm::vec3(-7.0f, 0.1f, 7.0f));
		m_StartingPoints.push_back(glm::vec3(7.0f, 0.1f, 7.0f));
		m_StartingPoints.push_back(glm::vec3(3.0f, 0.1f, 13.0f));
		m_StartingPoints.push_back(glm::vec3(7.0f, 0.1f, -7.0f));
		m_StartingPoints.push_back(glm::vec3(0.1f, 0.1f, 0.0f));
		m_StartingPoints.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

		ComputeTable(m_StartingPoints);
	}

	
	void Movement::ComputeTable(std::vector<glm::vec3> path)
	{
		m_ControlPoints.clear();
		m_PlotPoints.clear();
		m_FinalTable.clear();
		m_LastTableEntry = TableEntry{ 0.0f, 0.0f };

		//Calculate control points using starting points
		for (auto i = 1; i < path.size() - 1; ++i)
		{
			auto a = path[i] +
				(path[i + 1] - path[i - 1]) / 9.0f;

			auto b = path[i] -
				(path[i + 1] - path[i - 1]) / 9.0f;

			m_ControlPoints.emplace_back(b);
			m_ControlPoints.emplace_back(path[i]);
			m_ControlPoints.emplace_back(a);
		}

		//A line is drawn between each plot point to draw the curve

		for (auto i = 1; i < m_ControlPoints.size() - 3; i += 3)
		{
			//generate 100 points to draw a smooth curve using lines
			for (auto j = 0; j < 10; ++j)
			{
				//need a "u" in [0, 1] range 
				float u = j / 9.0f;

				glm::vec3 point = InterpolationFunc(u, m_ControlPoints[i], m_ControlPoints[i + 1], m_ControlPoints[i + 2], m_ControlPoints[i + 3]);

				m_PlotPoints.emplace_back(point);
			}
		}

		//array of arc length tables 
		std::vector<std::map<float, float>> tables;

		//build arc length tables using adaptive approach
		for (int i = 1; i < m_ControlPoints.size() - 3; i += 3)
		{
			std::map<float, float>  currentTable;
			currentTable[0.0f] = 0.0f;


			std::list<std::pair<float, float>> segmentList;
			segmentList.push_back(std::make_pair(0.0f, 1.0f));

			while (!segmentList.empty())
			{
				auto& const firstElement = segmentList.front();
				auto ua = firstElement.first;
				auto ub = firstElement.second;
				auto um = (ua + ub) * 0.5f;

				auto& P0 = m_ControlPoints[i];
				auto& P1 = m_ControlPoints[i + 1];
				auto& P2 = m_ControlPoints[i + 2];
				auto& P3 = m_ControlPoints[i + 3];

				auto A = (InterpolationFunc(ua, P0, P1, P2, P3) - InterpolationFunc(um, P0, P1, P2, P3));
				auto B = (InterpolationFunc(um, P0, P1, P2, P3) - InterpolationFunc(ub, P0, P1, P2, P3));
				auto C = (InterpolationFunc(ua, P0, P1, P2, P3) - InterpolationFunc(ub, P0, P1, P2, P3));

				auto d = length(A) + length(B) - length(C);

				if (d > ERROR_THRESHOLD || fabsf(ua - ub) > MAX_PARAM_INTERVAL)
				{
					segmentList.push_back(std::make_pair(ua, um));
					segmentList.push_back(std::make_pair(um, ub));
					segmentList.pop_front();
				}
				else
				{
					currentTable[um] = currentTable[ua] + length(A);
					currentTable[ub] = currentTable[um] + length(B);
					segmentList.pop_front();
				}
			}

			tables.push_back(currentTable);
		}


		//combine tables into one final table

		float maxLength = 0.0f;
		for (int tableIndex = 0; tableIndex < tables.size(); ++tableIndex)
		{
			for (auto const& currentElement : tables[tableIndex])
			{
				auto u = currentElement.first + tableIndex;
				auto length = currentElement.second + maxLength;

				TableEntry currentEntry = { u, length };

				m_FinalTable.emplace_back(currentEntry);
			}

			maxLength = m_FinalTable.back().length;
		}

		m_LastTableEntry = m_FinalTable.back();

		//Normalize table
		for (auto& element : m_FinalTable)
		{
			element.u /= m_LastTableEntry.u;
			element.length /= m_LastTableEntry.length;
		}

	}

}


