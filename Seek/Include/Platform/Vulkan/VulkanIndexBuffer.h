#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <volk.h>
#include <vk_mem_alloc.h>

namespace Seek
{
    class VulkanIndexBuffer
    {
    public:
        VulkanIndexBuffer(const uint32* data, uint32 count);
        ~VulkanIndexBuffer();

        inline uint32 GetCount() const { return m_Count; }

        inline VkBuffer GetHandle() const { return m_Handle; }
        inline VmaAllocation GetMemory() const { return m_Memory; }

    private:
        uint32 m_Count = 0;

        VkBuffer m_Handle = 0;
        VmaAllocation m_Memory = 0;
    };
}