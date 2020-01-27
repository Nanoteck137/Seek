#pragma once

#include "Seek/Renderer/RenderTarget.h"

#include "Platform/Vulkan/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanRenderPass.h"

#include <volk.h>
#include <vk_mem_alloc.h>

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
        void CreateDepthImage();
        void CreateFramebuffers();

    private:
        VulkanSwapchain* m_Swapchain = nullptr;
        VulkanRenderPass* m_RenderPass = nullptr;
        VkImage m_DepthImage = 0;
        VmaAllocation m_DepthMemory = 0;
        VkImageView m_DepthImageView = 0;
        std::vector<VkFramebuffer> m_Framebuffers;
    };
}