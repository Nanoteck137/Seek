#pragma once

#include "Seek/Renderer/Shader.h"

namespace Seek
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const String& filePath);
        OpenGLShader(const String& name, const String& vertexSource,
                     const String& fragmentSource);
        ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetUniformInt(const String& name, int32 value) override;
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

        virtual const String& GetName() const { return m_Name; }

    private:
        String ReadFile(const String& filePath);

        std::unordered_map<uint32, String> PreProcess(const String& source);

        void Compile(const std::unordered_map<uint32, String>& shaderSources);

    private:
        uint32 m_RendererID;
        String m_Name;
    };
}