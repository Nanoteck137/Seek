#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanRenderTargetSwapchain.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VulkanRenderTargetSwapchain::VulkanRenderTargetSwapchain(
        VulkanSwapchain* swapchain)
        : m_Swapchain(swapchain)
    {
        CreateRenderPass();
        CreateFramebuffers();
    }

    VulkanRenderTargetSwapchain::~VulkanRenderTargetSwapchain()
    {
        for (uint32 i = 0; i < m_Framebuffers.size(); i++)
        {
            vkDestroyFramebuffer(VulkanGraphicsContext::Get()->GetDevice(),
                                 m_Framebuffers[i], nullptr);
            m_Framebuffers[i] = 0;
        }

        if (m_RenderPass)
        {
            delete m_RenderPass;
            m_RenderPass = nullptr;
        }
    }

    VkFramebuffer VulkanRenderTargetSwapchain::GetCurrentFramebuffer() const
    {
        return m_Framebuffers[VulkanGraphicsContext::Get()->GetCurrentImage()];
    }

    void VulkanRenderTargetSwapchain::CreateRenderPass()
    {
        m_RenderPass = new VulkanRenderPass(m_Swapchain->GetFormat());
    }

    void VulkanRenderTargetSwapchain::CreateFramebuffers()
    {
        VkExtent2D extent = m_Swapchain->GetExtent();

        m_Framebuffers.resize(m_Swapchain->GetImageCount());
        for (uint32 i = 0; i < m_Swapchain->GetImageCount(); i++)
        {
            VkImageView attachments[1] = {m_Swapchain->GetImageViews()[i]};

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType =
                VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = m_RenderPass->GetHandle();
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.width = extent.width;
            framebufferCreateInfo.height = extent.height;
            framebufferCreateInfo.layers = 1;

            VkFramebuffer handle = 0;
            VK_CHECK(
                vkCreateFramebuffer(VulkanGraphicsContext::Get()->GetDevice(),
                                    &framebufferCreateInfo, nullptr, &handle));
            m_Framebuffers[i] = handle;
        }
    }
}