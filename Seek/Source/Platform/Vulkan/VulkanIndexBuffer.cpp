#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanIndexBuffer.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VulkanIndexBuffer::VulkanIndexBuffer(const uint32* data, uint32 count)
        : m_Count(count)
    {
        VmaAllocator allocator =
            VulkanGraphicsContext::Get()->GetMemoryAllocator();

        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = count * sizeof(uint32);
        bufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VK_CHECK(vmaCreateBuffer(allocator, &bufferCreateInfo, &allocInfo,
                                 &m_Handle, &m_Memory, nullptr));

        void* dataPtr;
        vmaMapMemory(allocator, m_Memory, &dataPtr);

        memcpy(dataPtr, data, count * sizeof(uint32));

        vmaUnmapMemory(allocator, m_Memory);
    }

    VulkanIndexBuffer::~VulkanIndexBuffer() {}
}