#include <Galactica.h>

class Sandbox : public Galactica::Application
{
	public:
		Sandbox()
		{

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