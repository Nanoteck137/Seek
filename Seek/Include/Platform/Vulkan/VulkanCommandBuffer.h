#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Platform/Vulkan/VulkanRenderPass.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanIndexBuffer.h"

#include <volk.h>

namespace Seek
{
    class VulkanCommandBuffer
    {
    public:
        VulkanCommandBuffer(uint32 count);
        ~VulkanCommandBuffer();

        void Begin();
        void End();

        // Commands
        void BeginRenderPass(VulkanRenderPass* renderPass,
                             VulkanFramebuffer* framebuffer);
        void EndRenderPass();

        void Clear();

        void BindPipeline(VulkanGraphicsPipeline* pipeline);
        void BindVertexBuffer(VulkanVertexBuffer* vertexBuffer);
        void BindIndexBuffer(VulkanIndexBuffer* indexBuffer);

        void DrawIndexed(uint32 count, uint32 firstIndex);

        inline VkCommandBuffer GetCurrentHandle() const
        {
            return m_CurrentCommandBuffer;
        }

    private:
        void CreateCommandPool();
        void CreateCommandBuffers();
        void AcquireNextBuffer();

    private:
        VkCommandPool m_CommandPool = 0;
        std::vector<VkCommandBuffer> m_Handles;
        VkCommandBuffer m_CurrentCommandBuffer;

        uint32 m_Count = 0;
        uint32 m_Index = -1;
    };
}