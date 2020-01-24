#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VulkanFramebuffer::VulkanFramebuffer(VulkanRenderPass* renderPass,
                                         VkImageView attachment, uint32 width,
                                         uint32 height)
    {
        VkDevice device = VulkanGraphicsContext::Get()->GetDevice();

        VkImageView attachments[1] = {attachment};

        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = renderPass->GetHandle();
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = attachments;
        framebufferCreateInfo.width = width;
        framebufferCreateInfo.height = height;
        framebufferCreateInfo.layers = 1;

        VK_CHECK(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr,
                                     &m_Handle));
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {
        if (m_Handle)
        {
            vkDestroyFramebuffer(VulkanGraphicsContext::Get()->GetDevice(),
                                 m_Handle, nullptr);
            m_Handle = 0;
        }
    }
}