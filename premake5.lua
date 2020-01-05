workspace "Seek"
    architecture "x86_64"
    startproject "Seek"

    configurations { "Debug", "Release", "Dist" }

    flags { "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Seek/Vendor/GLFW/include"
IncludeDir["GLAD"] = "Seek/Vendor/GLAD/include"
IncludeDir["imgui"] = "Seek/Vendor/imgui/"

-- Header Only Includes
IncludeDir["spdlog"] = "Seek/Vendor/spdlog/include/"
IncludeDir["glm"] = "Seek/Vendor/glm/"

group "Dependencies"
    include "Seek/Vendor"

group ""

project "Seek"
    location "Seek"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    pchheader "SeekPCH.h"
    pchsource "Seek/Source/Seek/SeekPCH.cpp"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files { 
        "%{prj.name}/Include/**.h", 
        "%{prj.name}/Source/**.cpp" 
    }

    includedirs {
        "%{prj.name}/Include/",

        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",

        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.imgui}",
    }

    links {
        "GLFW",
        "GLAD",
        "imgui",
        "opengl32.lib"
    }

    filter "system:windows"
        defines { "GLFW_INCLUDE_NONE" }

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