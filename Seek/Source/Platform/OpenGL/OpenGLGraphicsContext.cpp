#include "SeekPCH.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Seek
{
    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
    }

    void OpenGLGraphicsContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        SK_CORE_ASSERT(status, "Could not initialize GLAD");

        SK_CORE_INFO("-------------------------------------------");
        SK_CORE_INFO("  Initialized the OpenGL Graphics Context");
        SK_CORE_INFO("    OpenGL Version: {0}", glGetString(GL_VERSION));
        SK_CORE_INFO("    GLSL Version: {0}",
                     glGetString(GL_SHADING_LANGUAGE_VERSION));
        SK_CORE_INFO("    Graphics Vendor: {0}", glGetString(GL_VENDOR));
        SK_CORE_INFO("    Graphics Renderer: {0}", glGetString(GL_RENDERER));
        SK_CORE_INFO("-------------------------------------------");
    }

    void OpenGLGraphicsContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}