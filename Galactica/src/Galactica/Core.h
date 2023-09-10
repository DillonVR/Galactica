#pragma once

#ifdef GL_PLATFORM_WINDOWS
	#ifdef GL_BUILD_DLL
		#define GALACTICA_API _declspec(dllexport)
	#else
		#define GALACTICA_API _declspec(dllimport)
	#endif // GL_BUILD_DLL
#else
	#error Galactica only Supports windows!!!
#endif // GL_PLATFORM_WINDOWS

#ifdef GL_ENABLE_ASSERTS
	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define GL_ASSERT(x, ...){ if(!(x)) { GL_LOGGER_ERR(__VA_ARGS__);__debugbreak(); } }
	#define GL_CORE_ASSERT(x,...) { if(!(x)) { GL_LOGGER_ERR(__VA_ARGS__);__debugbreak(); } }
#else
	#define GL_ASSERT(x,...)
	#define GL_CORE_ASSERT(x,...)
#endif // GL_ENABLE_ASSERTS


#define BIT(x) (1 << x)