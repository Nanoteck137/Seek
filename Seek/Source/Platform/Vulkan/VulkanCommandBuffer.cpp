#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VulkanCommandBuffer::VulkanCommandBuffer(VkCommandPool pool, uint32 count)
        : m_CommandPool(pool), m_Count(count)
    {
        VkDevice device = VulkanGraphicsContext::Get()->GetDevice();

        VkCommandBufferAllocateInfo commandBufferAllocInfo = {};
        commandBufferAllocInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocInfo.commandPool = pool;
        commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocInfo.commandBufferCount = count;

        m_Handles.resize(count);
        VK_CHECK(vkAllocateCommandBuffers(device, &commandBufferAllocInfo,
                                          m_Handles.data()));
    }

    VulkanCommandBuffer::~VulkanCommandBuffer()
    {
        vkFreeCommandBuffers(VulkanGraphicsContext::Get()->GetDevice(),
                             m_CommandPool, m_Count, m_Handles.data());
    }

    void VulkanCommandBuffer::Begin()
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        VK_CHECK(vkBeginCommandBuffer(m_Handles[m_Index], &beginInfo));
    }

    void VulkanCommandBuffer::End()
    {
        VK_CHECK(vkEndCommandBuffer(m_Handles[m_Index]));

        m_Index++;
        m_Index = m_Index % m_Count;
    }
}