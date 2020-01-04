workspace "Seek"
    architecture "x86_64"
    startproject "Seek"

    configurations { "Debug", "Release", "Dist" }

    flags { "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Seek"
    location "Seek"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files { 
        "%{prj.name}/Include/**.h", 
        "%{prj.name}/Source/**.cpp" 
    }

    includedirs {
        "%{prj.name}/Include/",
        "Dependencies/Include/",

        "%{prj.name}/Vendor/spdlog/include",
    }

    filter "configurations:Debug"
        defines { "SK_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "SK_RELEASE" }
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines { "SK_DIST" }
        runtime "Release"
        optimize "On"