#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <volk.h>

namespace Seek
{
    class VulkanPipelineLayout
    {
    public:
        VulkanPipelineLayout();
        ~VulkanPipelineLayout();

        inline VkPipelineLayout GetHandle() const { return m_Handle; }
        inline VkDescriptorSetLayout GetDescriptorSetLayout() const
        {
            return m_DescriptorSetLayout;
        }

    private:
        VkPipelineLayout m_Handle = 0;
        VkDescriptorSetLayout m_DescriptorSetLayout = 0;
    };
}