#pragma once
#include "GLFW/glfw3.h"

namespace Galactica
{
	class Timestep
	{
	public:

		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		static float GetTime() { return glfwGetTime(); }

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }

		float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}
