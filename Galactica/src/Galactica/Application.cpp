#include "glpch.h"
#include "Application.h"

#include "Galactica/Events/ApplicationEvent.h"
#include "Galactica/Logger/Logger.h"

namespace Galactica {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	
	}

}