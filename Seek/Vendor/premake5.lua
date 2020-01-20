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

    VULAKN_SDK_PATH = os.getenv("VK_SDK_PATH")

    includedirs {
        "%{prj.name}/Source/",
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
    

project "glslang"
    location "glslang"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        -- glslang
        "%{prj.name}/glslang/OSDependent/Windows/ossource.cpp",
        "%{prj.name}/glslang/MachineIndependent/glslang.m4",
        "%{prj.name}/glslang/MachineIndependent/glslang.y",
        "%{prj.name}/glslang/MachineIndependent/glslang_tab.cpp",
        "%{prj.name}/glslang/MachineIndependent/attribute.cpp",
        "%{prj.name}/glslang/MachineIndependent/Constant.cpp",
        "%{prj.name}/glslang/MachineIndependent/iomapper.cpp",
        "%{prj.name}/glslang/MachineIndependent/InfoSink.cpp",
        "%{prj.name}/glslang/MachineIndependent/Initialize.cpp",
        "%{prj.name}/glslang/MachineIndependent/IntermTraverse.cpp",
        "%{prj.name}/glslang/MachineIndependent/Intermediate.cpp",
        "%{prj.name}/glslang/MachineIndependent/ParseContextBase.cpp",
        "%{prj.name}/glslang/MachineIndependent/ParseHelper.cpp",
        "%{prj.name}/glslang/MachineIndependent/PoolAlloc.cpp",
        "%{prj.name}/glslang/MachineIndependent/RemoveTree.cpp",
        "%{prj.name}/glslang/MachineIndependent/Scan.cpp",
        "%{prj.name}/glslang/MachineIndependent/ShaderLang.cpp",
        "%{prj.name}/glslang/MachineIndependent/SymbolTable.cpp",
        "%{prj.name}/glslang/MachineIndependent/Versions.cpp",
        "%{prj.name}/glslang/MachineIndependent/intermOut.cpp",
        "%{prj.name}/glslang/MachineIndependent/limits.cpp",
        "%{prj.name}/glslang/MachineIndependent/linkValidate.cpp",
        "%{prj.name}/glslang/MachineIndependent/parseConst.cpp",
        "%{prj.name}/glslang/MachineIndependent/reflection.cpp",
        "%{prj.name}/glslang/MachineIndependent/preprocessor/Pp.cpp",
        "%{prj.name}/glslang/MachineIndependent/preprocessor/PpAtom.cpp",
        "%{prj.name}/glslang/MachineIndependent/preprocessor/PpContext.cpp",
        "%{prj.name}/glslang/MachineIndependent/preprocessor/PpScanner.cpp",
        "%{prj.name}/glslang/MachineIndependent/preprocessor/PpTokens.cpp",
        "%{prj.name}/glslang/MachineIndependent/propagateNoContraction.cpp",
        "%{prj.name}/glslang/GenericCodeGen/CodeGen.cpp",
        "%{prj.name}/glslang/GenericCodeGen/Link.cpp",
        "%{prj.name}/glslang/Public/ShaderLang.h",
        "%{prj.name}/glslang/Include/arrays.h",
        "%{prj.name}/glslang/Include/BaseTypes.h",
        "%{prj.name}/glslang/Include/Common.h",
        "%{prj.name}/glslang/Include/ConstantUnion.h",
        "%{prj.name}/glslang/Include/InfoSink.h",
        "%{prj.name}/glslang/Include/InitializeGlobals.h",
        "%{prj.name}/glslang/Include/intermediate.h",
        "%{prj.name}/glslang/Include/PoolAlloc.h",
        "%{prj.name}/glslang/Include/ResourceLimits.h",
        "%{prj.name}/glslang/Include/revision.h",
        "%{prj.name}/glslang/Include/ShHandle.h",
        "%{prj.name}/glslang/Include/Types.h",
        "%{prj.name}/glslang/MachineIndependent/attribute.h",
        "%{prj.name}/glslang/MachineIndependent/glslang_tab.cpp.h",
        "%{prj.name}/glslang/MachineIndependent/gl_types.h",
        "%{prj.name}/glslang/MachineIndependent/Initialize.h",
        "%{prj.name}/glslang/MachineIndependent/iomapper.h",
        "%{prj.name}/glslang/MachineIndependent/LiveTraverser.h",
        "%{prj.name}/glslang/MachineIndependent/localintermediate.h",
        "%{prj.name}/glslang/MachineIndependent/ParseHelper.h",
        "%{prj.name}/glslang/MachineIndependent/reflection.h",
        "%{prj.name}/glslang/MachineIndependent/RemoveTree.h",
        "%{prj.name}/glslang/MachineIndependent/Scan.h",
        "%{prj.name}/glslang/MachineIndependent/ScanContext.h",
        "%{prj.name}/glslang/MachineIndependent/SymbolTable.h",
        "%{prj.name}/glslang/MachineIndependent/Versions.h",
        "%{prj.name}/glslang/MachineIndependent/parseVersions.h",
        "%{prj.name}/glslang/MachineIndependent/propagateNoContraction.h",
        "%{prj.name}/glslang/MachineIndependent/preprocessor/PpContext.h",
        "%{prj.name}/glslang/MachineIndependent/preprocessor/PpTokens.h",

        -- OGLCompilersDLL
        "%{prj.name}/OGLCompilersDLL/InitializeDll.cpp", 
        "%{prj.name}/OGLCompilersDLL/InitializeDll.h",

        -- SPIRV
        "%{prj.name}/SPIRV/InReadableOrder.cpp",
        "%{prj.name}/SPIRV/GlslangToSpv.cpp",
        "%{prj.name}/SPIRV/Logger.cpp",
        "%{prj.name}/SPIRV/SpvBuilder.cpp",
        "%{prj.name}/SPIRV/SpvPostProcess.cpp",
        "%{prj.name}/SPIRV/doc.cpp",
        "%{prj.name}/SPIRV/SpvTools.cpp",
        "%{prj.name}/SPIRV/disassemble.cpp",
        "%{prj.name}/SPIRV/SPVRemapper.cpp",
        "%{prj.name}/SPIRV/doc.cpp",
        "%{prj.name}/SPIRV/bitutils.h",
        "%{prj.name}/SPIRV/spirv.hpp",
        "%{prj.name}/SPIRV/GLSL.std.450.h",
        "%{prj.name}/SPIRV/GLSL.ext.EXT.h",
        "%{prj.name}/SPIRV/GLSL.ext.KHR.h",
        "%{prj.name}/SPIRV/GlslangToSpv.h",
        "%{prj.name}/SPIRV/hex_float.h",
        "%{prj.name}/SPIRV/Logger.h",
        "%{prj.name}/SPIRV/SpvBuilder.h",
        "%{prj.name}/SPIRV/spvIR.h",
        "%{prj.name}/SPIRV/doc.h",
        "%{prj.name}/SPIRV/SpvTools.h",
        "%{prj.name}/SPIRV/disassemble.h",
        "%{prj.name}/SPIRV/GLSL.ext.AMD.h",
        "%{prj.name}/SPIRV/GLSL.ext.NV.h",
        "%{prj.name}/SPIRV/SPVRemapper.h",
        "%{prj.name}/SPIRV/doc.h",

        -- hlsl
        "%{prj.name}/hlsl/hlslAttributes.cpp",
        "%{prj.name}/hlsl/hlslParseHelper.cpp",
        "%{prj.name}/hlsl/hlslScanContext.cpp",
        "%{prj.name}/hlsl/hlslOpMap.cpp",
        "%{prj.name}/hlsl/hlslTokenStream.cpp",
        "%{prj.name}/hlsl/hlslGrammar.cpp",
        "%{prj.name}/hlsl/hlslParseables.cpp",
        "%{prj.name}/hlsl/hlslAttributes.h",
        "%{prj.name}/hlsl/hlslParseHelper.h",
        "%{prj.name}/hlsl/hlslTokens.h",
        "%{prj.name}/hlsl/hlslScanContext.h",
        "%{prj.name}/hlsl/hlslOpMap.h",
        "%{prj.name}/hlsl/hlslTokenStream.h",
        "%{prj.name}/hlsl/hlslGrammar.h",
        "%{prj.name}/hlsl/hlslParseables.h",
    }

    VULAKN_SDK_PATH = os.getenv("VK_SDK_PATH")

    includedirs {
        "%{prj.name}/"
    }

    defines {
        "ENABLE_HLSL",
        "AMD_EXTENSIONS",
        "NV_EXTENSIONS",
        -- TODO(patrik): Move to windows only
        "GLSLANG_OSINCLUDE_WIN32",
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

project "V-EZ"
    location "V-EZ"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/Source/Compiler/GLSLCompiler.cpp",
        "%{prj.name}/Source/Compiler/GLSLCompiler.h",
        "%{prj.name}/Source/Compiler/ResourceLimits.cpp",
        "%{prj.name}/Source/Compiler/ResourceLimits.h",
        "%{prj.name}/Source/Compiler/SPIRVReflection.cpp",
        "%{prj.name}/Source/Compiler/SPIRVReflection.h",

        "%{prj.name}/Source/Core/Buffer.cpp",
        "%{prj.name}/Source/Core/Buffer.h",
        "%{prj.name}/Source/Core/BufferView.cpp",
        "%{prj.name}/Source/Core/BufferView.h",
        "%{prj.name}/Source/Core/CommandBuffer.cpp",
        "%{prj.name}/Source/Core/CommandBuffer.h",
        "%{prj.name}/Source/Core/CommandPool.cpp",
        "%{prj.name}/Source/Core/CommandPool.h",
        "%{prj.name}/Source/Core/DescriptorPool.cpp",
        "%{prj.name}/Source/Core/DescriptorPool.h",
        "%{prj.name}/Source/Core/DescriptorSetLayout.cpp",
        "%{prj.name}/Source/Core/DescriptorSetLayout.h",
        "%{prj.name}/Source/Core/DescriptorSetLayoutCache.cpp",
        "%{prj.name}/Source/Core/DescriptorSetLayoutCache.h",
        "%{prj.name}/Source/Core/Device.cpp",
        "%{prj.name}/Source/Core/Device.h",
        "%{prj.name}/Source/Core/Fence.h",
        "%{prj.name}/Source/Core/Framebuffer.cpp",
        "%{prj.name}/Source/Core/Framebuffer.h",
        "%{prj.name}/Source/Core/GraphicsState.cpp",
        "%{prj.name}/Source/Core/GraphicsState.h",
        "%{prj.name}/Source/Core/Image.cpp",
        "%{prj.name}/Source/Core/Image.h",
        "%{prj.name}/Source/Core/ImageView.cpp",
        "%{prj.name}/Source/Core/ImageView.h",
        "%{prj.name}/Source/Core/Instance.cpp",
        "%{prj.name}/Source/Core/Instance.h",
        "%{prj.name}/Source/Core/PhysicalDevice.h",
        "%{prj.name}/Source/Core/Pipeline.cpp",
        "%{prj.name}/Source/Core/Pipeline.h",
        "%{prj.name}/Source/Core/PipelineBarriers.cpp",
        "%{prj.name}/Source/Core/PipelineBarriers.h",
        "%{prj.name}/Source/Core/PipelineCache.cpp",
        "%{prj.name}/Source/Core/PipelineCache.h",
        "%{prj.name}/Source/Core/Queue.cpp",
        "%{prj.name}/Source/Core/Queue.h",
        "%{prj.name}/Source/Core/RenderPassCache.cpp",
        "%{prj.name}/Source/Core/RenderPassCache.h",
        "%{prj.name}/Source/Core/ResourceBindings.cpp",
        "%{prj.name}/Source/Core/ResourceBindings.h",
        "%{prj.name}/Source/Core/ShaderModule.cpp",
        "%{prj.name}/Source/Core/ShaderModule.h",
        "%{prj.name}/Source/Core/StreamDecoder.cpp",
        "%{prj.name}/Source/Core/StreamDecoder.h",
        "%{prj.name}/Source/Core/StreamEncoder.cpp",
        "%{prj.name}/Source/Core/StreamEncoder.h",
        "%{prj.name}/Source/Core/Swapchain.cpp",
        "%{prj.name}/Source/Core/Swapchain.h",
        "%{prj.name}/Source/Core/SyncPrimitivesPool.cpp",
        "%{prj.name}/Source/Core/SyncPrimitivesPool.h",
        "%{prj.name}/Source/Core/VertexInputFormat.cpp",
        "%{prj.name}/Source/Core/VertexInputFormat.h",

        "%{prj.name}/Source/Utility/Macros.h",
        "%{prj.name}/Source/Utility/MemoryStream.cpp",
        "%{prj.name}/Source/Utility/MemoryStream.h",
        "%{prj.name}/Source/Utility/ObjectLookup.cpp",
        "%{prj.name}/Source/Utility/ObjectLookup.h",
        "%{prj.name}/Source/Utility/SpinLock.h",
        "%{prj.name}/Source/Utility/ThreadPool.cpp",
        "%{prj.name}/Source/Utility/ThreadPool.h",
        "%{prj.name}/Source/Utility/VkHelpers.h",

        "%{prj.name}/Source/VEZ.def",
        "%{prj.name}/Source/VEZ.cpp",
        "%{prj.name}/Source/VEZ.h",
        "%{prj.name}/Source/VEZ_ext.cpp",
        "%{prj.name}/Source/VEZ_ext.h"
    }

    VULAKN_SDK_PATH = os.getenv("VK_SDK_PATH")

    includedirs {
        "%{prj.name}/Source/",
        "glslang/",
        "SPIRV-Cross/",
        "VulkanMemoryAllocator/src/",

        VULAKN_SDK_PATH .. "/Include",
    }

    defines {
        "ENABLE_HLSL"
    }

    libdirs {
        VULAKN_SDK_PATH .. "/Lib",
    }

    links {
        "SPIRV-Cross",
        "glslang",

        "vulkan-1.lib"
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

