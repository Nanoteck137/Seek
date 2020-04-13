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

    glm::vec2 WindowsInput::GetMousePositionImpl()
    {
        Window& window = Application::Get().GetWindow();
        GLFWwindow* handle = static_cast<GLFWwindow*>(window.GetNativeWindow());

        float64 xPos;
        float64 yPos;
        glfwGetCursorPos(handle, &xPos, &yPos);

        yPos = window.GetHeight() - yPos;

        return glm::vec2((float32)xPos, (float32)yPos);
    }
}