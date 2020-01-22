#pragma once

#include "Seek/Renderer/GraphicsContext.h"

#include <volk.h>

struct GLFWwindow;

#define VK_CHECK(x)                                                            \
    {                                                                          \
        VkResult res = (x);                                                    \
        SK_CORE_ASSERT(res == VK_SUCCESS, "Vulkan Result: {0}",                \
                       Seek::GetVulkanErrorString(res))                        \
    }

namespace Seek
{
    extern String GetVulkanErrorString(VkResult errorCode);

    class VulkanGraphicsContext : public GraphicsContext
    {
    public:
        VulkanGraphicsContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void SwapBuffers() override;

        inline VkInstance GetInstance() const { return m_Instance; }
        inline VkSurfaceKHR GetSurface() const { return m_Surface; }

        inline uint32 GetGraphicsFamilyIndex() const
        {
            return m_GraphicsQueueFamilyIndex;
        }

        inline uint32 GetPresentFamilyIndex() const
        {
            return m_PresentQueueFamilyIndex;
        }

        inline VkPhysicalDevice GetPhysicalDevice() const
        {
            return m_PhysicalDevice;
        }

        inline VkDevice GetDevice() const { return m_Device; }
        inline VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
        inline VkQueue GetPresentQueue() const { return m_PresentQueue; }

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