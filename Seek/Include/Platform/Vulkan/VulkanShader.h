#pragma once

#include "Seek/Renderer/Shader.h"

#include <volk.h>

namespace Seek
{
    class VulkanShader : public Shader
    {
    public:
        VulkanShader(const String& vertPath, const String& fragPath);
        ~VulkanShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetUniformInt(const String& name, int32 value) override;
        virtual void SetUnifromIntArray(const String& name, int32* values,
                                        uint32 count) override;
        virtual void SetUniformFloat(const String& name,
                                     float32 value) override;
        virtual void SetUniformFloat2(const String& name,
                                      const glm::vec2& value) override;
        virtual void SetUniformFloat3(const String& name,
                                      const glm::vec3& value) override;
        virtual void SetUniformFloat4(const String& name,
                                      const glm::vec4& value) override;
        virtual void SetUniformMatrix4(const String& name,
                                       const glm::mat4& matrix) override;

        virtual const String& GetName() const override;

        inline VkShaderModule GetVertexShaderModule()
        {
            return m_VertexShaderModule;
        }

        inline VkShaderModule GetFragmentShaderModule()
        {
            return m_FragmentShaderModule;
        }

    private:
        VkShaderModule m_VertexShaderModule = 0;
        VkShaderModule m_FragmentShaderModule = 0;
    };
}