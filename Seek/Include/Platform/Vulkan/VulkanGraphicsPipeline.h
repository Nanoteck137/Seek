#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Platform/Vulkan/VulkanRenderPass.h"
#include "Platform/Vulkan/VulkanPipelineLayout.h"
#include "Platform/Vulkan/VulkanShader.h"

#include <glm/glm.hpp>

#include <volk.h>

namespace Seek
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;

        bool operator==(const Vertex& other) const
        {
            return position == other.position && color == other.color;
        }
    };

    class VulkanGraphicsPipeline
    {
    public:
        VulkanGraphicsPipeline(VulkanRenderPass* renderPass,
                               VulkanShader* shader,
                               VulkanPipelineLayout* layout);
        ~VulkanGraphicsPipeline();

        inline VkPipeline GetHandle() const { return m_Handle; }

    private:
        VkPipeline m_Handle = 0;
    };
}
