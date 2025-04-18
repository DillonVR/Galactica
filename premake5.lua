workspace "Galactica"
	architecture "x64"
	configurations{
		"Debug",
		"Release",
		"Dist"
	}
	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include Dir relative to the root folder

IncludeDir = {}
IncludeDir["GLFW"] = "Galactica/Vendor/GLFW/include"
IncludeDir["Glad"] = "Galactica/Vendor/Glad/include"
IncludeDir["ImGui"] = "Galactica/Vendor/imgui"
IncludeDir["Assimp"] = "Galactica/Vendor/assimp/include"

include "Galactica/Vendor/GLFW"
include "Galactica/Vendor/Glad"
include "Galactica/Vendor/imgui"
include "Galactica/Vendor/assimp"

project "Galactica"
	location "Galactica"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "glpch.h"
	pchsource "Galactica/src/glpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.inl",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Vendor/stb_image/**.h",
		"%{prj.name}/Vendor/stb_image/**.cpp",
		"%{prj.name}/Vendor/glm/glm/**.hpp",
		"%{prj.name}/Vendor/glm/glm/**.inl"
	}

	-- Any Vendors we need to include
	includedirs{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Assimp}"
	}

	links{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"Assimp",
		"ImGui"
	}

	filter { "system:windows"}
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"GL_PLATFORM_WINDOWS",
			"GL_BUILD_DLL",
			"_CRT_SECURE_NO_WARNINGS"
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "GL_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "GL_RELEASE"
		buildoptions "/MD"
		symbols "On"

	filter "configurations:Dist"
		defines "GL_DIST"
		buildoptions "/MD"
		symbols "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- Any files that need to be included
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	-- Any Vendors we need to include
	includedirs{
		"Galactica/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Assimp}"

	}

	links { "Galactica" }

	filter { "system:windows"}
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"GL_PLATFORM_WINDOWS",
			"GL_BUILD_DLL",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "GL_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "GL_RELEASE"
		buildoptions "/MD"
		symbols "On"

	filter "configurations:Dist"
		defines "GL_DIST"
		buildoptions "/MD"
		symbols "On"