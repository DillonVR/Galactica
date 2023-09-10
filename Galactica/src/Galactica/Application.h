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


	private:

		bool OnWindowClose(WindowCloseEvent& e);

		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
	};

	//defined in client App
	Application* CreateApplication();

}


