#pragma once

#include "Seek/Renderer/RenderTarget.h"

#include "Platform/Vulkan/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanRenderPass.h"

#include <volk.h>

namespace Seek
{
    class VulkanRenderTargetSwapchain : public RenderTarget
    {
    public:
        VulkanRenderTargetSwapchain(VulkanSwapchain* swapchain);
        ~VulkanRenderTargetSwapchain();

        inline VulkanRenderPass* GetRenderPass() const { return m_RenderPass; }
        VkFramebuffer GetCurrentFramebuffer() const;

    private:
        void CreateRenderPass();
        void CreateFramebuffers();

    private:
        VulkanSwapchain* m_Swapchain = nullptr;
        VulkanRenderPass* m_RenderPass = nullptr;
        std::vector<VkFramebuffer> m_Framebuffers;
    };
}