#pragma once

#include "Core.h"

#include "Events/Event.h"
#include "Galactica/Events/ApplicationEvent.h"
#include "Galactica/LayerStack.h"

#include "Window.h"

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

		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
	};

	//defined in client App
	Application* CreateApplication();

}


