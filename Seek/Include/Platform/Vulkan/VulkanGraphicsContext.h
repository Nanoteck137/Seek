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
        virtual void Shutdown() override;

        virtual void SwapBuffers() override;

    private:
        void CreateInstance();
        void CreateDebugMessenger();
        void CreateSurface();

        void PickPhysicalDevice();
        void GetQueueFamiles();
        void CreateLogicalDevice();
        void GetQueues();

    private:
        GLFWwindow* m_WindowHandle = nullptr;

        VkInstance m_Instance = 0;
        VkDebugUtilsMessengerEXT m_DebugMessenger = 0;
        VkSurfaceKHR m_Surface = 0;

        uint32 m_GraphicsQueueFamilyIndex = 0;
        uint32 m_PresentQueueFamilyIndex = 0;

        VkPhysicalDevice m_PhysicalDevice = 0;
        VkDevice m_Device = 0;

        VkQueue m_GraphicsQueue = 0;
        VkQueue m_PresentQueue = 0;
    };
}