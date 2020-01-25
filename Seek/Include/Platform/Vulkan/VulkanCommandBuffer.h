#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <volk.h>

namespace Seek
{
    class VulkanCommandBuffer
    {
    public:
        VulkanCommandBuffer(VkCommandPool pool, uint32 count);
        ~VulkanCommandBuffer();

        void Begin();
        void End();

        inline VkCommandBuffer GetCurrentHandle() const
        {
            return m_Handles[m_Index];
        }

    private:
        VkCommandPool m_CommandPool = 0;
        std::vector<VkCommandBuffer> m_Handles;

        uint32 m_Count = 0;
        uint32 m_Index = 0;
    };
}