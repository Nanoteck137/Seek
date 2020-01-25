#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanCommandQueue.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VulkanCommandQueue::VulkanCommandQueue(VkQueue queue) : m_Queue(queue) {}
    VulkanCommandQueue::~VulkanCommandQueue() {}

    void VulkanCommandQueue::Submit(VulkanCommandBuffer* commandBuffer)
    {
        VkCommandBuffer handle = commandBuffer->GetCurrentHandle();

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &handle;
        submitInfo.signalSemaphoreCount = 0;

        VK_CHECK(vkQueueSubmit(m_Queue, 1, &submitInfo, 0));
    }

    void VulkanCommandQueue::WaitIdle() { VK_CHECK(vkQueueWaitIdle(m_Queue)); }
}