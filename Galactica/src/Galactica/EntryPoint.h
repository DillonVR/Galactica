#pragma once

#ifdef GL_PLATFORM_WINDOWS

extern Galactica::Application* Galactica::CreateApplication();

int main(int argc, char** argv)
{
	printf("Hello from space");
	auto app = Galactica::CreateApplication();
	app->Run();
	delete app;
}

#endif // GL_PLATFORM_WINODWS
