workspace "Seek"
    architecture "x86_64"
    startproject "Sandbox"

    configurations { "Debug", "Release", "Dist" }

    flags { "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
VULKAN_SDK_PATH = os.getenv("VK_SDK_PATH")

IncludeDir = {}
IncludeDir["GLFW"] = "Seek/Vendor/GLFW/include"
IncludeDir["GLAD"] = "Seek/Vendor/GLAD/include"
IncludeDir["imgui"] = "Seek/Vendor/imgui"
IncludeDir["stb"] = "Seek/Vendor/stb"
IncludeDir["Box2D"] = "Seek/Vendor/Box2D/include"

IncludeDir["volk"] = "Seek/Vendor/volk"
IncludeDir["SPIRVCross"] = "Seek/Vendor/SPIRV-Cross"
IncludeDir["Vulkan"] = VULKAN_SDK_PATH .. "/Include"

IncludeDir["tinyobjloader"] = "Seek/Vendor/tinyobjloader"

-- Header Only Includes
IncludeDir["spdlog"] = "Seek/Vendor/spdlog/include"
IncludeDir["glm"] = "Seek/Vendor/glm"
IncludeDir["VulkanMemoryAllocator"] = "Seek/Vendor/VulkanMemoryAllocator/src"

group "Dependencies"
    include "Seek/Vendor"

group ""

project "Seek"
    location "Seek"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "%{IncludeDir.stb}",
        "%{IncludeDir.Box2D}",

        "%{IncludeDir.volk}",
        "%{IncludeDir.VulkanMemoryAllocator}",
        "%{IncludeDir.SPIRVCross}",
        "%{IncludeDir.Vulkan}",

        "%{IncludeDir.tinyobjloader}",
    }

    links {
        "GLFW",
        "GLAD",
        "imgui",
        "stb",
        "Box2D",
        "volk",
        "SPIRV-Cross",
        "tinyobjloader",

        "opengl32.lib",
        "Xaudio2.lib"
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

        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.Box2D}",
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