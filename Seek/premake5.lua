outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

target_dir = "%{wks.location}/bin/" .. outputdir .. "/%{prj.name}"
obj_dir = "%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}"

IncludeDir = {}
IncludeDir["GLFW"] = "Vendor/GLFW/include"
IncludeDir["GLAD"] = "Vendor/GLAD/include"
IncludeDir["imgui"] = "Vendor/imgui"
IncludeDir["stb"] = "Vendor/stb"
IncludeDir["Box2D"] = "Vendor/Box2D/include"

-- Header Only Includes
IncludeDir["spdlog"] = "Vendor/spdlog/include/"
IncludeDir["glm"] = "Vendor/glm/"

group "Dependencies"
    include "Vendor"

group ""

project "Seek"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    pchheader "SeekPCH.h"
    pchsource "Source/Seek/SeekPCH.cpp"
    
    targetdir (target_dir)
    objdir (obj_dir)

    files { 
        "Include/**.h", 
        "Source/**.cpp" 
    }

    includedirs {
        "Include/",

        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",

        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.Box2D}",
    }

    links {
        "GLFW",
        "GLAD",
        "imgui",
        "stb",
        "Box2D",
        "opengl32.lib",
        "xaudio2.lib"
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

