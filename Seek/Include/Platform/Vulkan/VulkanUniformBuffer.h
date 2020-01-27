#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <volk.h>
#include <vk_mem_alloc.h>

namespace Seek
{
    class VulkanUniformBuffer
    {
    public:
        VulkanUniformBuffer(const void* data, uint32 size);
        ~VulkanUniformBuffer();

        inline VkBuffer GetHandle() const { return m_Handle; }
        inline VmaAllocation GetMemory() const { return m_Memory; }

    private:
        VkBuffer m_Handle;
        VmaAllocation m_Memory;
    };
}