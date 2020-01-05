#pragma once

#include "Seek/Renderer/Shader.h"

namespace Seek
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const String& vertexSource, const String& fragmentSource);
        ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetUniformInt(const String& name, int32 value) override;
        virtual void SetUniformMatrix4(const String& name,
                                       const glm::mat4& matrix) override;

    private:
        uint32 m_RendererID;
    };
}