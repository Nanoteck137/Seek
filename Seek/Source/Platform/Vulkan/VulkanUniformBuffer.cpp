#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanUniformBuffer.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VulkanUniformBuffer::VulkanUniformBuffer(const void* data, uint32 size)
    {
        VmaAllocator allocator =
            VulkanGraphicsContext::Get()->GetMemoryAllocator();

        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VK_CHECK(vmaCreateBuffer(allocator, &bufferCreateInfo, &allocInfo,
                                 &m_Handle, &m_Memory, nullptr));

        void* dataPtr;
        vmaMapMemory(allocator, m_Memory, &dataPtr);

        memcpy(dataPtr, data, size);

        vmaUnmapMemory(allocator, m_Memory);
    }

    VulkanUniformBuffer::~VulkanUniformBuffer()
    {
        if (m_Memory)
        {
            vmaFreeMemory(VulkanGraphicsContext::Get()->GetMemoryAllocator(),
                          m_Memory);
            m_Memory = 0;
        }

        if (m_Handle)
        {
            vkDestroyBuffer(VulkanGraphicsContext::Get()->GetDevice(), m_Handle,
                            nullptr);
            m_Handle = 0;
        }
    }
}