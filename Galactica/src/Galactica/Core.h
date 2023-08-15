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
