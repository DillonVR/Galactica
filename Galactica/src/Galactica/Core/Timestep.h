#pragma once
#include "GLFW/glfw3.h"

namespace Galactica
{
	class Timestep
	{
	public:

		Timestep(float time = 0.0f)
			:	m_Time(time),
				m_TotalTime(time)
		{
		}

		static float GetTime() { return static_cast<float>(glfwGetTime()); }

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }

		float GetMilliseconds() const { return m_Time * 1000.0f; }

		float GetTotal() const { return m_TotalTime; }

	private:
		float m_Time;

		float m_TotalTime;
	};
}
