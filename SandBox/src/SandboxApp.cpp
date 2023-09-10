#include "glpch.h"
#include <Galactica.h>

class ExampleLayer : public Galactica::Layer
{
public:

	ExampleLayer() : Layer("Expamle")
	{
	}

	void OnUpdate() override
	{
		//GL_LOGGER_INFO("Example layer -- Update");
	}

	void OnEvent(Galactica::Event& event) override
	{
		//GL_LOGGER_INFO(event);
	}

private:

};



class Sandbox : public Galactica::Application
{
	public:
		Sandbox()
		{
			PushLayer(new ExampleLayer());
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