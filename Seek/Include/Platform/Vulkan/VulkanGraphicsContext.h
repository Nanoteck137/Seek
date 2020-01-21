#pragma once

#include "Seek/Renderer/GraphicsContext.h"

#include <volk.h>

struct GLFWwindow;

namespace Seek
{
    class VulkanGraphicsContext : public GraphicsContext
    {
    public:
        VulkanGraphicsContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        void CreateInstance();
        void CreateDebugMessenger();
        void PickPhysicalDevice();
        void CreateLogicalDevice();

    private:
        GLFWwindow* m_WindowHandle = nullptr;

        VkInstance m_Instance = 0;
        VkDebugUtilsMessengerEXT m_DebugMessenger = 0;

        VkPhysicalDevice m_PhysicalDevice = 0;
        VkDevice m_Device = 0;
    };
}