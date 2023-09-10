#pragma once

#include "Galactica/Core.h"
#include "Galactica/Events/Event.h"

namespace Galactica
{
	class GALACTICA_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() { return DebugName; }

	protected:
		std::string DebugName;
	};
}


