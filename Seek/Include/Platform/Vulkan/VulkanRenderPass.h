#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <volk.h>

namespace Seek
{
    class VulkanRenderPass
    {
    public:
        VulkanRenderPass(VkFormat format);
        ~VulkanRenderPass();

        inline VkRenderPass GetHandle() const { return m_Handle; }

    private:
        VkRenderPass m_Handle = 0;
    };
}