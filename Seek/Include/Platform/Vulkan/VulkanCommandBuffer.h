#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Platform/Vulkan/VulkanRenderPass.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanFramebuffer.h"

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

        // Commands
        void BeginRenderPass(VulkanRenderPass* renderPass,
                             VulkanFramebuffer* framebuffer);
        void EndRenderPass();

        void Clear();

        void BindPipeline(VulkanGraphicsPipeline* pipeline);
        void BindVertexBuffer(VkBuffer vertexBuffer);
        void BindIndexBuffer(VkBuffer indexBuffer);

        void DrawIndexed(uint32 count, uint32 firstIndex);

        inline VkCommandBuffer GetCurrentHandle() const
        {
            return m_CurrentCommandBuffer;
        }

    private:
        void AcquireNextBuffer();

    private:
        VkCommandPool m_CommandPool = 0;
        std::vector<VkCommandBuffer> m_Handles;
        VkCommandBuffer m_CurrentCommandBuffer;

        uint32 m_Count = 0;
        uint32 m_Index = -1;
    };
}