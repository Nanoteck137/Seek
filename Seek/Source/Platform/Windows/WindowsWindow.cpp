#include "SeekPCH.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Platform/OpenGL/OpenGLGraphicsContext.h"

#include "Seek/Events/ApplicationEvent.h"
#include "Seek/Events/KeyEvents.h"
#include "Seek/Events/MouseEvent.h"

#include "Seek/Debug/Instrumentor.h"

#include <GLFW/glfw3.h>

namespace Seek
{
    static bool s_GLFWInitialized = false;

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props) { Init(props); }

    WindowsWindow::~WindowsWindow() { Shutdown(); }

    void WindowsWindow::Init(const WindowProps& props)
    {
        SK_PROFILE_FUNCTION();

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        SK_CORE_INFO("Creating window '{0}' ({1}, {2})", props.Title,
                     props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            int32 success = glfwInit();
            SK_CORE_ASSERT(success, "Could not initialize GLFW");

            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(props.Width, props.Height,
                                    props.Title.c_str(), nullptr, nullptr);

        m_Context = new OpenGLGraphicsContext(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32 width,
                                               int32 height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32 key,
                                        int32 scancode, int32 action,
                                        int32 mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32 code) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(code);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window,
                                                int32 button, int32 action,
                                                int32 mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            float64 mouseX = 0.0f;
            float64 mouseY = 0.0f;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            mouseY = -(mouseY - data.Height);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button, (float32)mouseX,
                                                  (float32)mouseY);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button, (float32)mouseX,
                                                   (float32)mouseY);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, float64 xOffset,
                                           float64 yOffset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float32)xOffset, (float32)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, float64 xPos,
                                              float64 yPos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            yPos = -(yPos - data.Height);
            MouseMovedEvent event((float32)xPos, (float32)yPos);
            data.EventCallback(event);
        });
    }

    void WindowsWindow::Shutdown()
    {
        SK_PROFILE_FUNCTION();

        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        SK_PROFILE_FUNCTION();

        glfwPollEvents();

        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        SK_PROFILE_FUNCTION();

        glfwSwapInterval(enabled ? 1 : 0);

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::GetVSync() const { return m_Data.VSync; }
}