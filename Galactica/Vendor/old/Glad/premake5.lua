project "Glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Include/glad/glad.h",
		"Include/KHR/khrplatform.h",
		"src/glad.c"
	}
	
	includedirs
	{
		"Include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

    
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
