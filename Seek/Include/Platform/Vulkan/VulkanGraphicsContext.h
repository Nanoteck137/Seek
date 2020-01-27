#pragma once

#include "Seek/Renderer/GraphicsContext.h"

#include "Platform/Vulkan/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanRenderTargetSwapchain.h"

#include <volk.h>

#include <vk_mem_alloc.h>

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

        virtual void Present() override;

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

        inline VmaAllocator GetMemoryAllocator() const
        {
            return m_MemoryAllocator;
        }

        inline VulkanSwapchain* GetSwapchain() const { return m_Swapchain; }

        inline VkSemaphore GetImageAvailableSemaphore() const
        {
            return m_ImageAvailableSemaphore;
        }

        inline VkSemaphore GetRenderFinishedSemaphore() const
        {
            return m_RenderFinishedSemaphore;
        }

        inline uint32 GetCurrentImage() { return m_CurrentImage; }

        inline VulkanRenderTargetSwapchain* GetRenderTarget() const
        {
            return m_RenderTarget;
        }

    public:
        static inline VulkanGraphicsContext* Get() { return s_Instance; }

    private:
        void CreateInstance();
        void CreateDebugMessenger();
        void CreateSurface();

        void PickPhysicalDevice();
        void GetQueueFamiles();
        void CreateLogicalDevice();
        void GetQueues();

        void CreateMemoryAllocator();
        void CreateSemaphores();

        void GetNextIndex();

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

        VmaAllocator m_MemoryAllocator = 0;

        VulkanSwapchain* m_Swapchain = nullptr;

        VkSemaphore m_ImageAvailableSemaphore = 0;
        VkSemaphore m_RenderFinishedSemaphore = 0;

        uint32 m_CurrentImage = 0;

        VulkanRenderTargetSwapchain* m_RenderTarget;

    private:
        static VulkanGraphicsContext* s_Instance;
    };
}