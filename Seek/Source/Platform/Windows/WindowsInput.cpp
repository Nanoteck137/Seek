#include "SeekPCH.h"
#include "Platform/Windows/WindowsInput.h"

#include "Seek/App/Application.h"

#include <GLFW/glfw3.h>

namespace Seek
{
    Input* Input::s_Instance = new WindowsInput();

    bool WindowsInput::IsKeyPressedImpl(int keycode)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(
            Application::Get().GetWindow().GetNativeWindow());

        int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(
            Application::Get().GetWindow().GetNativeWindow());

        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    float WindowsInput::GetMouseXImpl()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(
            Application::Get().GetWindow().GetNativeWindow());

        double xpos;
        double ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return (float)xpos;
    }

    float WindowsInput::GetMouseYImpl()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(
            Application::Get().GetWindow().GetNativeWindow());

        double xpos;
        double ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return (float)ypos;
    }
}