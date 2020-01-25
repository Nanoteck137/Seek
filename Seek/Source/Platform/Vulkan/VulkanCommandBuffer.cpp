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

        AcquireNextBuffer();
    }

    VulkanCommandBuffer::~VulkanCommandBuffer()
    {
        vkFreeCommandBuffers(VulkanGraphicsContext::Get()->GetDevice(),
                             m_CommandPool, m_Count, m_Handles.data());
    }

    void VulkanCommandBuffer::Begin()
    {
        AcquireNextBuffer();

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        VK_CHECK(vkBeginCommandBuffer(m_Handles[m_Index], &beginInfo));
    }

    void VulkanCommandBuffer::End()
    {
        VK_CHECK(vkEndCommandBuffer(m_Handles[m_Index]));
    }

    void VulkanCommandBuffer::BeginRenderPass(VulkanRenderPass* renderPass,
                                              VulkanFramebuffer* framebuffer)
    {
        VulkanSwapchain* swapchain =
            VulkanGraphicsContext::Get()->GetSwapchain();

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = renderPass->GetHandle();
        renderPassBeginInfo.framebuffer = framebuffer->GetHandle();
        renderPassBeginInfo.renderArea.extent = swapchain->GetExtent();
        renderPassBeginInfo.renderArea.offset = {0, 0};

        vkCmdBeginRenderPass(m_CurrentCommandBuffer, &renderPassBeginInfo,
                             VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanCommandBuffer::EndRenderPass()
    {
        vkCmdEndRenderPass(m_CurrentCommandBuffer);
    }

    void VulkanCommandBuffer::Clear()
    {
        VulkanSwapchain* swapchain =
            VulkanGraphicsContext::Get()->GetSwapchain();

        VkClearValue clearColor = {0.5f, 0.2f, 0.5f, 1.0f};

        VkClearAttachment attachment = {};
        attachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        attachment.colorAttachment = 0;
        attachment.clearValue = clearColor;

        VkClearRect clearRect = {};
        clearRect.rect.offset = {0, 0};
        clearRect.rect.extent = swapchain->GetExtent();
        clearRect.baseArrayLayer = 0;
        clearRect.layerCount = 1;

        vkCmdClearAttachments(m_CurrentCommandBuffer, 1, &attachment, 1,
                              &clearRect);
    }

    void VulkanCommandBuffer::BindPipeline(VulkanGraphicsPipeline* pipeline)
    {
        vkCmdBindPipeline(m_CurrentCommandBuffer,
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline->GetHandle());
    }

    void VulkanCommandBuffer::BindVertexBuffer(VkBuffer vertexBuffer)
    {
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(m_CurrentCommandBuffer, 0, 1, vertexBuffers,
                               offsets);
    }

    void VulkanCommandBuffer::BindIndexBuffer(VkBuffer indexBuffer)
    {
        vkCmdBindIndexBuffer(m_CurrentCommandBuffer, indexBuffer, 0,
                             VK_INDEX_TYPE_UINT32);
    }

    void VulkanCommandBuffer::DrawIndexed(uint32 count, uint32 firstIndex)
    {
        vkCmdDrawIndexed(m_CurrentCommandBuffer, count, 1, firstIndex, 0, 0);
    }

    void VulkanCommandBuffer::AcquireNextBuffer()
    {
        m_Index++;
        m_Index = m_Index % m_Count;
        m_CurrentCommandBuffer = m_Handles[m_Index];
    }
}