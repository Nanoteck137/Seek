#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include <volk.h>

namespace Seek
{
    class VulkanCommandQueue
    {
    public:
        VulkanCommandQueue(VkQueue queue);
        ~VulkanCommandQueue();

        void Submit(VulkanCommandBuffer* commandBuffer);
        void WaitExecutionFinished();
        void WaitIdle();

    private:
        void CreateFence();

    private:
        VkQueue m_Queue = 0;
        VkFence m_ExecutedFence = 0;
    };
}