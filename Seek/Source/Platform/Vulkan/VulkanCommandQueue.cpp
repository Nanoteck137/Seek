#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanCommandQueue.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VulkanCommandQueue::VulkanCommandQueue(VkQueue queue) : m_Queue(queue)
    {
        CreateFence();
    }

    VulkanCommandQueue::~VulkanCommandQueue()
    {
        if (m_ExecutedFence)
        {
            vkDestroyFence(VulkanGraphicsContext::Get()->GetDevice(),
                           m_ExecutedFence, nullptr);
            m_ExecutedFence = 0;
        }
    }

    void VulkanCommandQueue::CreateFence()
    {
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VK_CHECK(vkCreateFence(VulkanGraphicsContext::Get()->GetDevice(),
                               &fenceCreateInfo, nullptr, &m_ExecutedFence));
    }

    void VulkanCommandQueue::Submit(VulkanCommandBuffer* commandBuffer)
    {
        VkCommandBuffer handle = commandBuffer->GetCurrentHandle();

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &handle;
        submitInfo.signalSemaphoreCount = 0;

        VK_CHECK(vkResetFences(VulkanGraphicsContext::Get()->GetDevice(), 1,
                               &m_ExecutedFence));
        VK_CHECK(vkQueueSubmit(m_Queue, 1, &submitInfo, m_ExecutedFence));
    }

    void VulkanCommandQueue::WaitExecutionFinished()
    {
        VK_CHECK(vkWaitForFences(VulkanGraphicsContext::Get()->GetDevice(), 1,
                                 &m_ExecutedFence, VK_TRUE, UINT64_MAX));
    }

    void VulkanCommandQueue::WaitIdle() { VK_CHECK(vkQueueWaitIdle(m_Queue)); }
}