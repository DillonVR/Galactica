#pragma once

#ifdef GL_PLATFORM_WINDOWS

extern Galactica::Application* Galactica::CreateApplication();

int main(int argc, char** argv)
{
	GL_LOGGER_INFO("INFO LOG Init");
	GL_LOGGER_ERR("ERROR LOG Init");
	GL_LOGGER_EVT("EVENT LOG Init");

	auto app = Galactica::CreateApplication();
	app->Run();
	delete app;
}

#endif // GL_PLATFORM_WINODWS
