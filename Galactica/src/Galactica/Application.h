#pragma once

#include "Core.h"

#include "Events/Event.h"
#include "Galactica/Events/ApplicationEvent.h"
#include "Galactica/LayerStack.h"

#include "Window.h"
#include "ImGui/ImGuiLayer.h"

namespace Galactica {

	class GALACTICA_API Application
	{
	public:

		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);

		static Application& Get() { return *s_Instance; }

		Window& GetWindow() const { return *m_Window; }

		
	private:

		bool OnWindowClose(WindowCloseEvent& e);

		inline static Application* s_Instance = nullptr;

		float m_LastFrameTime = 0.0f;

		bool m_Running = true;

		std::unique_ptr<Window> m_Window;

		ImGuiLayer* m_imGUILayer;

		LayerStack m_LayerStack;
	};

	//defined in client App
	Application* CreateApplication();

}


