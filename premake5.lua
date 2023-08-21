workspace "Galactica"
	architecture "x64"
	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Galactica"
	location "Galactica"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	-- Any Vendors we need to include
	--include{}

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
		symbols "On"

	filter "configurations:Release"
		defines "GL_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "GL_DIST"
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
		"Galactica/src"
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
		symbols "On"

	filter "configurations:Release"
		defines "GL_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "GL_DIST"
		symbols "On"