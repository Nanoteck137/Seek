#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanPipelineLayout.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VulkanPipelineLayout::VulkanPipelineLayout()
    {
        VkDevice device = VulkanGraphicsContext::Get()->GetDevice();

        VkDescriptorSetLayoutBinding uboLayoutBinding = {};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
            uboLayoutBinding, samplerLayoutBinding};
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = bindings.size();
        layoutInfo.pBindings = bindings.data();

        VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr,
                                             &m_DescriptorSetLayout));

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;

        pipelineLayoutCreateInfo.setLayoutCount = 1;
        pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;

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

        if (m_DescriptorSetLayout)
        {
            vkDestroyDescriptorSetLayout(
                VulkanGraphicsContext::Get()->GetDevice(),
                m_DescriptorSetLayout, nullptr);
            m_DescriptorSetLayout = 0;
        }
    }
}