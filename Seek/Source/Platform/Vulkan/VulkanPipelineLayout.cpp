#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanPipelineLayout.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VulkanPipelineLayout::VulkanPipelineLayout()
    {
        VkDevice device = VulkanGraphicsContext::Get()->GetDevice();

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;

        VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo,
                                        nullptr, &m_Handle));
    }

    VulkanPipelineLayout::~VulkanPipelineLayout()
    {
        if (m_Handle)
        {
            vkDestroyPipelineLayout(VulkanGraphicsContext::Get()->GetDevice(),
                                    m_Handle, nullptr);
            m_Handle = 0;
        }
    }
}