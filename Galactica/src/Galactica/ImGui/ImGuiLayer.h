#pragma once

#include "Galactica/Layer.h"

namespace Galactica
{
	class GALACTICA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnUpdate() override;

		void OnEvent(Event& event) override;

		void OnAttach() override;

		void OnDetach() override;


	private:

		bool show_demo_window;

		float m_time = 0.0f;
	};
}
