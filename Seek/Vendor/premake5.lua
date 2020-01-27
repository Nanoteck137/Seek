project "GLFW"
    location "GLFW"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/include/GLFW/glfw3.h",
        "%{prj.name}/include/GLFW/glfw3native.h",
        "%{prj.name}/src/glfw_config.h",
        "%{prj.name}/src/context.c",
        "%{prj.name}/src/init.c",
        "%{prj.name}/src/input.c",
        "%{prj.name}/src/monitor.c",
        "%{prj.name}/src/vulkan.c",
        "%{prj.name}/src/window.c"
    }
    
    filter "system:linux"
        pic "On"

        systemversion "latest"
        staticruntime "On"

        files {
            "%{prj.name}/src/x11_init.c",
            "%{prj.name}/src/x11_monitor.c",
            "%{prj.name}/src/x11_window.c",
            "%{prj.name}/src/xkb_unicode.c",
            "%{prj.name}/src/posix_time.c",
            "%{prj.name}/src/posix_thread.c",
            "%{prj.name}/src/glx_context.c",
            "%{prj.name}/src/egl_context.c",
            "%{prj.name}/src/osmesa_context.c",
            "%{prj.name}/src/linux_joystick.c"
        }

        defines {
            "_GLFW_X11"
        }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        files {
            "%{prj.name}/src/win32_init.c",
            "%{prj.name}/src/win32_joystick.c",
            "%{prj.name}/src/win32_monitor.c",
            "%{prj.name}/src/win32_time.c",
            "%{prj.name}/src/win32_thread.c",
            "%{prj.name}/src/win32_window.c",
            "%{prj.name}/src/wgl_context.c",
            "%{prj.name}/src/egl_context.c",
            "%{prj.name}/src/osmesa_context.c"
        }

        defines { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"

project "GLAD"
    location "GLAD"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/glad.c",
        "%{prj.name}/include/glad/glad.h",
        "%{prj.name}/include/KHR/khrplatform.h",
    }

    includedirs {
        "%{prj.name}/include/",
    }
    
    filter "system:linux"
        pic "On"

        systemversion "latest"
        staticruntime "On"

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"

project "imgui"
    location "imgui"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/imconfig.h",
        "%{prj.name}/imgui.h",
        "%{prj.name}/imgui.cpp",
        "%{prj.name}/imgui_draw.cpp",
        "%{prj.name}/imgui_internal.h",
        "%{prj.name}/imgui_widgets.cpp",
        "%{prj.name}/imstb_rectpack.h",
        "%{prj.name}/imstb_textedit.h",
        "%{prj.name}/imstb_truetype.h",
        "%{prj.name}/imgui_demo.cpp"
    }
    
    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"

project "stb"
    location "stb"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        -- stb_image
        "%{prj.name}/stb_image.h",
        "%{prj.name}/stb_image.cpp",
    }
    
    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"

project "Box2D"
    location "Box2D"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        -- Include Files
        "%{prj.name}/include/box2d/b2_block_allocator.h",
        "%{prj.name}/include/box2d/b2_body.h",
        "%{prj.name}/include/box2d/b2_broad_phase.h",
        "%{prj.name}/include/box2d/b2_chain_shape.h",
        "%{prj.name}/include/box2d/b2_circle_shape.h",
        "%{prj.name}/include/box2d/b2_collision.h",
        "%{prj.name}/include/box2d/b2_contact.h",
        "%{prj.name}/include/box2d/b2_contact_manager.h",
        "%{prj.name}/include/box2d/b2_distance.h",
        "%{prj.name}/include/box2d/b2_distance_joint.h",
        "%{prj.name}/include/box2d/b2_draw.h",
        "%{prj.name}/include/box2d/b2_dynamic_tree.h",
        "%{prj.name}/include/box2d/b2_edge_shape.h",
        "%{prj.name}/include/box2d/b2_fixture.h",
        "%{prj.name}/include/box2d/b2_friction_joint.h",
        "%{prj.name}/include/box2d/b2_gear_joint.h",
        "%{prj.name}/include/box2d/b2_growable_stack.h",
        "%{prj.name}/include/box2d/b2_joint.h",
        "%{prj.name}/include/box2d/b2_math.h",
        "%{prj.name}/include/box2d/b2_motor_joint.h",
        "%{prj.name}/include/box2d/b2_mouse_joint.h",
        "%{prj.name}/include/box2d/b2_polygon_shape.h",
        "%{prj.name}/include/box2d/b2_prismatic_joint.h",
        "%{prj.name}/include/box2d/b2_pulley_joint.h",
        "%{prj.name}/include/box2d/b2_revolute_joint.h",
        "%{prj.name}/include/box2d/b2_rope.h",
        "%{prj.name}/include/box2d/b2_rope_joint.h",
        "%{prj.name}/include/box2d/b2_settings.h",
        "%{prj.name}/include/box2d/b2_shape.h",
        "%{prj.name}/include/box2d/b2_stack_allocator.h",
        "%{prj.name}/include/box2d/b2_time_of_impact.h",
        "%{prj.name}/include/box2d/b2_timer.h",
        "%{prj.name}/include/box2d/b2_time_step.h",
        "%{prj.name}/include/box2d/b2_weld_joint.h",
        "%{prj.name}/include/box2d/b2_wheel_joint.h",
        "%{prj.name}/include/box2d/b2_world.h",
        "%{prj.name}/include/box2d/b2_world_callbacks.h",
        "%{prj.name}/include/box2d/box2d.h",

        -- Source Files
        "%{prj.name}/src/collision/b2_broad_phase.cpp",
        "%{prj.name}/src/collision/b2_chain_shape.cpp",
        "%{prj.name}/src/collision/b2_circle_shape.cpp",
        "%{prj.name}/src/collision/b2_collide_circle.cpp",
        "%{prj.name}/src/collision/b2_collide_edge.cpp",
        "%{prj.name}/src/collision/b2_collide_polygon.cpp",
        "%{prj.name}/src/collision/b2_collision.cpp",
        "%{prj.name}/src/collision/b2_distance.cpp",
        "%{prj.name}/src/collision/b2_dynamic_tree.cpp",
        "%{prj.name}/src/collision/b2_edge_shape.cpp",
        "%{prj.name}/src/collision/b2_polygon_shape.cpp",
        "%{prj.name}/src/collision/b2_time_of_impact.cpp",
        "%{prj.name}/src/common/b2_block_allocator.cpp",
        "%{prj.name}/src/common/b2_draw.cpp",
        "%{prj.name}/src/common/b2_math.cpp",
        "%{prj.name}/src/common/b2_settings.cpp",
        "%{prj.name}/src/common/b2_stack_allocator.cpp",
        "%{prj.name}/src/common/b2_timer.cpp",
        "%{prj.name}/src/dynamics/b2_body.cpp",
        "%{prj.name}/src/dynamics/b2_chain_circle_contact.cpp",
        "%{prj.name}/src/dynamics/b2_chain_circle_contact.h",
        "%{prj.name}/src/dynamics/b2_chain_polygon_contact.cpp",
        "%{prj.name}/src/dynamics/b2_chain_polygon_contact.h",
        "%{prj.name}/src/dynamics/b2_circle_contact.cpp",
        "%{prj.name}/src/dynamics/b2_circle_contact.h",
        "%{prj.name}/src/dynamics/b2_contact.cpp",
        "%{prj.name}/src/dynamics/b2_contact_manager.cpp",
        "%{prj.name}/src/dynamics/b2_contact_solver.cpp",
        "%{prj.name}/src/dynamics/b2_contact_solver.h",
        "%{prj.name}/src/dynamics/b2_distance_joint.cpp",
        "%{prj.name}/src/dynamics/b2_edge_circle_contact.cpp",
        "%{prj.name}/src/dynamics/b2_edge_circle_contact.h",
        "%{prj.name}/src/dynamics/b2_edge_polygon_contact.cpp",
        "%{prj.name}/src/dynamics/b2_edge_polygon_contact.h",
        "%{prj.name}/src/dynamics/b2_fixture.cpp",
        "%{prj.name}/src/dynamics/b2_friction_joint.cpp",
        "%{prj.name}/src/dynamics/b2_gear_joint.cpp",
        "%{prj.name}/src/dynamics/b2_island.cpp",
        "%{prj.name}/src/dynamics/b2_island.h",
        "%{prj.name}/src/dynamics/b2_joint.cpp",
        "%{prj.name}/src/dynamics/b2_motor_joint.cpp",
        "%{prj.name}/src/dynamics/b2_mouse_joint.cpp",
        "%{prj.name}/src/dynamics/b2_polygon_circle_contact.cpp",
        "%{prj.name}/src/dynamics/b2_polygon_circle_contact.h",
        "%{prj.name}/src/dynamics/b2_polygon_contact.cpp",
        "%{prj.name}/src/dynamics/b2_polygon_contact.h",
        "%{prj.name}/src/dynamics/b2_prismatic_joint.cpp",
        "%{prj.name}/src/dynamics/b2_pulley_joint.cpp",
        "%{prj.name}/src/dynamics/b2_revolute_joint.cpp",
        "%{prj.name}/src/dynamics/b2_rope_joint.cpp",
        "%{prj.name}/src/dynamics/b2_weld_joint.cpp",
        "%{prj.name}/src/dynamics/b2_wheel_joint.cpp",
        "%{prj.name}/src/dynamics/b2_world.cpp",
        "%{prj.name}/src/dynamics/b2_world_callbacks.cpp",
        "%{prj.name}/src/rope/b2_rope.cpp",
    }

    includedirs {
        "%{prj.name}/include/",
        "%{prj.name}/src/",
    }
    
    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"


project "volk"
    location "volk"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/volk.c",
        "%{prj.name}/volk.h",
    }

    VULKAN_SDK_PATH = os.getenv("VK_SDK_PATH")

    includedirs {
        "%{prj.name}/",
        VULKAN_SDK_PATH .. "/Include"
    }
    
    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"

project "SPIRV-Cross"
    location "SPIRV-Cross"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {

        "%{prj.name}/spirv.h",
        "%{prj.name}/spirv.hpp",
        "%{prj.name}/GLSL.std.450.h",
        "%{prj.name}/spirv_common.hpp",
        "%{prj.name}/spirv_cross_containers.hpp",
        "%{prj.name}/spirv_cross_error_handling.hpp",
        "%{prj.name}/spirv_cross.hpp",
        "%{prj.name}/spirv_cross.cpp",
        "%{prj.name}/spirv_parser.hpp",
        "%{prj.name}/spirv_parser.cpp",
        "%{prj.name}/spirv_cross_parsed_ir.hpp",
        "%{prj.name}/spirv_cross_parsed_ir.cpp",
        "%{prj.name}/spirv_cfg.hpp",
        "%{prj.name}/spirv_cfg.cpp",
        "%{prj.name}/spirv_cross_c.cpp",
        "%{prj.name}/spirv_cross_c.h",
        "%{prj.name}/spirv_glsl.cpp",
        "%{prj.name}/spirv_glsl.hpp",
        "%{prj.name}/spirv_cpp.cpp",
        "%{prj.name}/spirv_cpp.hpp",
        "%{prj.name}/spirv_msl.cpp",
        "%{prj.name}/spirv_msl.hpp",
        "%{prj.name}/spirv_hlsl.cpp",
        "%{prj.name}/spirv_hlsl.hpp",
        "%{prj.name}/spirv_reflect.cpp",
        "%{prj.name}/spirv_reflect.hpp",
        "%{prj.name}/spirv_cross_util.cpp",
        "%{prj.name}/spirv_cross_util.hpp",
    }

    includedirs {
        "%{prj.name}/",
    }

    defines {
        "ENABLE_HLSL"
    }
    
    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"

project "tinyobjloader"
    location "tinyobjloader"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/tiny_obj_loader.h",
        "%{prj.name}/tiny_obj_loader.cc",
    }
    
    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"