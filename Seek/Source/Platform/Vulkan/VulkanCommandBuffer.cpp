#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Platform/Vulkan/VulkanRenderTargetSwapchain.h"

namespace Seek
{
    VulkanCommandBuffer::VulkanCommandBuffer(uint32 count) : m_Count(count)
    {
        CreateCommandPool();
        CreateCommandBuffers();

        AcquireNextBuffer();
    }

    VulkanCommandBuffer::~VulkanCommandBuffer()
    {
        vkFreeCommandBuffers(VulkanGraphicsContext::Get()->GetDevice(),
                             m_CommandPool, m_Count, m_Handles.data());

        if (m_CommandPool)
        {
            vkDestroyCommandPool(VulkanGraphicsContext::Get()->GetDevice(),
                                 m_CommandPool, nullptr);
            m_CommandPool = 0;
        }
    }

    void VulkanCommandBuffer::CreateCommandPool()
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.flags =
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolCreateInfo.queueFamilyIndex =
            VulkanGraphicsContext::Get()->GetGraphicsFamilyIndex();

        VK_CHECK(vkCreateCommandPool(VulkanGraphicsContext::Get()->GetDevice(),
                                     &commandPoolCreateInfo, nullptr,
                                     &m_CommandPool));
    }

    void VulkanCommandBuffer::CreateCommandBuffers()
    {
        VkCommandBufferAllocateInfo commandBufferAllocInfo = {};
        commandBufferAllocInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocInfo.commandPool = m_CommandPool;
        commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocInfo.commandBufferCount = m_Count;

        m_Handles.resize(m_Count);
        VK_CHECK(vkAllocateCommandBuffers(
            VulkanGraphicsContext::Get()->GetDevice(), &commandBufferAllocInfo,
            m_Handles.data()));
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

    void VulkanCommandBuffer::BeginRenderPass(RenderTarget* renderTarget)
    {
        // TODO(patrik): Remove
        VulkanRenderTargetSwapchain* renderTargetSwapchain =
            (VulkanRenderTargetSwapchain*)renderTarget;
        VulkanSwapchain* swapchain =
            VulkanGraphicsContext::Get()->GetSwapchain();

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass =
            renderTargetSwapchain->GetRenderPass()->GetHandle();
        renderPassBeginInfo.framebuffer =
            renderTargetSwapchain->GetCurrentFramebuffer();
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
        VkClearValue depthClear = {};
        depthClear.depthStencil = {1.0f, 0};

        VkClearAttachment colorAttachment = {};
        colorAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorAttachment.colorAttachment = 0;
        colorAttachment.clearValue = clearColor;

        VkClearAttachment depthAttachment = {};
        depthAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        depthAttachment.colorAttachment = 0;
        depthAttachment.clearValue = depthClear;

        std::array<VkClearAttachment, 2> attachments = {colorAttachment,
                                                        depthAttachment};

        VkClearRect clearRect = {};
        clearRect.rect.offset = {0, 0};
        clearRect.rect.extent = swapchain->GetExtent();
        clearRect.baseArrayLayer = 0;
        clearRect.layerCount = 1;

        vkCmdClearAttachments(m_CurrentCommandBuffer, attachments.size(),
                              attachments.data(), 1, &clearRect);
    }

    void VulkanCommandBuffer::BindPipeline(VulkanGraphicsPipeline* pipeline)
    {
        vkCmdBindPipeline(m_CurrentCommandBuffer,
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline->GetHandle());
    }

    void VulkanCommandBuffer::BindVertexBuffer(VulkanVertexBuffer* vertexBuffer)
    {
        VkBuffer vertexBuffers[1] = {vertexBuffer->GetHandle()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(m_CurrentCommandBuffer, 0, 1, vertexBuffers,
                               offsets);
    }

    void VulkanCommandBuffer::BindIndexBuffer(VulkanIndexBuffer* indexBuffer)
    {
        vkCmdBindIndexBuffer(m_CurrentCommandBuffer, indexBuffer->GetHandle(),
                             0, VK_INDEX_TYPE_UINT32);
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