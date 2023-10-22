#include "glpch.h"
#include "Application.h"

#include <fstream>

#include <GLFW/glfw3.h>

#include "Galactica/Vendor/glm/glm/glm.hpp"

#include "Galactica/Vendor/stb_image/stb_image.h"
#include "Renderer/CameraControl.h"
#include "Renderer/Model.h"

#include "Renderer/Shader.h"

namespace Galactica {

#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

	//Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		GL_CORE_ASSERT(!s_Instance, "Appliclacion Already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_imGUILayer = new ImGuiLayer();
		PushOverLayer(m_imGUILayer);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		stbi_set_flip_vertically_on_load(true);
		glEnable(GL_DEPTH_TEST);

		

		while (m_Running)
		{
			glClearColor(1.0, 1.0, 1.0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			float time = Timestep::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
			}

			m_imGUILayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_imGUILayer->End();

			m_Window->OnUpdate();

		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
		//GL_LOGGER_EVT(e.ToString());
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverLayer(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}
