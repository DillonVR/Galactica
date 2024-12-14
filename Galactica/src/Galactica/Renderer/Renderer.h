#pragma once

#include "CameraControl.h"
#include "Model.h"
#include "Shader.h"


namespace Galactica::Renderer
{
	class GALACTICA_API Renderer
	{
		public:

		

	
		void Setup(const char* vertexPath, const char* fragmentPath);
		void Draw(Model* light, std::vector<Model*> scene,CameraControl* cam, bool debug);
	};
		
	
}

