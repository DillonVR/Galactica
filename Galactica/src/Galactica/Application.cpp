#include "glpch.h"
#include "Application.h"

#include <fstream>
#include "Animation\StepTimer.h"
#include "Galactica/Vendor/glm/glm/glm.hpp"
#include "Galactica/Vendor/stb_image/stb_image.h"
#include "Renderer/CameraControl.h"
#include "Renderer/Model.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"

#include <GLFW/glfw3.h>

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
			glClearColor(0.1, 0.5, 0.8, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			float time = Timestep::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			Tick();
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

	void Application::Tick()
	{
		m_timer.Tick([&]()
			{
				Update(m_timer);
			});

		m_Window->OnUpdate();
	}

	void Application::Update(Galactica::StepTimer const& timer)
	{
		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate(timer);
		}

		m_imGUILayer->Begin();
		for (Layer* layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}
		m_imGUILayer->End();
		
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}
