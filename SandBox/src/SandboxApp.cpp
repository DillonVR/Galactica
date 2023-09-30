#include "glpch.h"
#include <Galactica.h>


class ExampleLayer : public Galactica::Layer
{
public:

	ExampleLayer() : Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//GL_LOGGER_INFO("Example layer -- Update");
	}

	void OnEvent(Galactica::Event& event) override
	{
		GL_LOGGER_INFO(event.ToString());
	}

private:

};



class Sandbox : public Galactica::Application
{
	public:
		Sandbox()
		{
			PushLayer(new ExampleLayer());
			PushOverLayer(new Galactica::ImGuiLayer());
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