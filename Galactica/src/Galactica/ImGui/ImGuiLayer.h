#pragma once

#include "imgui.h"
#include "Galactica/Layer.h"
#include "Galactica/Events/ApplicationEvent.h"
#include "Galactica/Events/KeyEvent.h"
#include "Galactica/Events/MouseEvent.h"

namespace Galactica
{
	class GALACTICA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		// void OnUpdate(Timestep ts) override;

		void OnEvent(Event& event) override;

		virtual void OnImGuiRender() override;

		void OnAttach() override;

		void OnDetach() override;

		void Begin();

		void End();




	private:

		static ImVec2 my_color;

		bool show_demo_window;

		float m_time = 0.0f;

	};
}
