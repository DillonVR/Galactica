#include "glpch.h"

#include <Galactica.h>

#include "AnimationLayer.h"



class Sandbox : public Galactica::Application
{
	public:
		Sandbox()
		{
			PushLayer(new AnimationLayer());

		}
		~Sandbox() 
		{

		}

	private:

};


Galactica::Application* Galactica::CreateApplication()
{
	return new Sandbox;
}