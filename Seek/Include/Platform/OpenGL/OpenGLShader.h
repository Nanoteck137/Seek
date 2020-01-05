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

    private:
        uint32 m_RendererID;
    };
}