workspace "Sandbox"
    architecture "x86_64"
    startproject "Sandbox"

    configurations { "Debug", "Release", "Dist" }

    flags { "MultiProcessorCompile" }

group "Engine"
    include "Seek"
group ""

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files { 
        "%{prj.name}/Include/**.h", 
        "%{prj.name}/Source/**.cpp"
    }

    includedirs {
        "%{prj.name}/Include/",
        "Seek/Include/",

        "Seek/%{IncludeDir.spdlog}",
        "Seek/%{IncludeDir.glm}",
        "Seek/%{IncludeDir.entt}",

        "Seek/%{IncludeDir.imgui}",
        "Seek/%{IncludeDir.Box2D}",
    }

    links {
        "Seek",
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