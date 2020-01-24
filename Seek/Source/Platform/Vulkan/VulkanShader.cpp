#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanShader.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

#include "Seek/System/FileSystem.h"

namespace Seek
{
    static VkShaderModule LoadShaderCode(VkDevice device,
                                         const String& filepath)
    {
        Buffer buffer = FileSystem::ReadAllBuffer(filepath);

        VkShaderModuleCreateInfo shaderCreateInfo = {};
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.pCode = (const uint32*)buffer.Data;
        shaderCreateInfo.codeSize = buffer.Size;

        VkShaderModule shaderModule = 0;
        VK_CHECK(vkCreateShaderModule(device, &shaderCreateInfo, nullptr,
                                      &shaderModule));

        buffer.Free();

        return shaderModule;
    }

    VulkanShader::VulkanShader(const String& vertPath, const String& fragPath)
    {
        VkDevice device = VulkanGraphicsContext::Get()->GetDevice();
        m_VertexShaderModule = LoadShaderCode(device, vertPath);
        m_FragmentShaderModule = LoadShaderCode(device, fragPath);
    }

    VulkanShader::~VulkanShader()
    {
        if (m_VertexShaderModule)
        {
            vkDestroyShaderModule(VulkanGraphicsContext::Get()->GetDevice(),
                                  m_VertexShaderModule, nullptr);
            m_VertexShaderModule = 0;
        }

        if (m_FragmentShaderModule)
        {
            vkDestroyShaderModule(VulkanGraphicsContext::Get()->GetDevice(),
                                  m_FragmentShaderModule, nullptr);
            m_FragmentShaderModule = 0;
        }
    }

    void VulkanShader::Bind() const {}
    void VulkanShader::Unbind() const {}

    void VulkanShader::SetUniformInt(const String& name, int32 value) {}
    void VulkanShader::SetUnifromIntArray(const String& name, int32* values,
                                          uint32 count)
    {
    }
    void VulkanShader::SetUniformFloat(const String& name, float32 value) {}
    void VulkanShader::SetUniformFloat2(const String& name,
                                        const glm::vec2& value)
    {
    }
    void VulkanShader::SetUniformFloat3(const String& name,
                                        const glm::vec3& value)
    {
    }
    void VulkanShader::SetUniformFloat4(const String& name,
                                        const glm::vec4& value)
    {
    }
    void VulkanShader::SetUniformMatrix4(const String& name,
                                         const glm::mat4& matrix)
    {
    }

    const String& VulkanShader::GetName() const
    {
        return "TODO Give vulkan shaders names";
    }
}