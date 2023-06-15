workspace "smol"
	architecture "x86_64"
	startproject "smol"

	configurations { "Debug", "Release" }

	flags { "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

-- smol executable

project "smol"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-obj/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp",
	}

	linkoptions {
	}

	filter "system:windows"
		systemversion "latest"

		defines {
            "SMOL_PLATFORM_WIN32"
        }

		filter "configurations:Debug*"
			defines "SMOL_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release*"
			defines "SMOL_RELEASE"
			runtime "Release"
			optimize "on"
			