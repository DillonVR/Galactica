#pragma once

#include "Core.h"

namespace Galactica {

	class GALACTICA_API Application
	{
	public:

		Application();
		virtual ~Application();

		void Run();
	};

	//defined in client App
	Application* CreateApplication();

}


