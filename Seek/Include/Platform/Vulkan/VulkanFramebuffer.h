#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Platform/Vulkan/VulkanRenderPass.h"

#include <volk.h>

namespace Seek
{
    class VulkanFramebuffer
    {
    public:
        VulkanFramebuffer(VulkanRenderPass* renderPass, VkImageView attachment,
                          uint32 width, uint32 height);
        ~VulkanFramebuffer();

        inline VkFramebuffer GetHandle() const { return m_Handle; }

    private:
        uint32 m_Width;
        uint32 m_Height;

        VkFramebuffer m_Handle;
        VulkanRenderPass* m_RenderPass;
    };
}