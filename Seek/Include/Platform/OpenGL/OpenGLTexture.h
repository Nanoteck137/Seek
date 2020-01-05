#pragma once

#include "Seek/Renderer/Texture.h"

namespace Seek
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const String& path);
        ~OpenGLTexture2D();

        virtual uint32 GetWidth() const override { return m_Width; }
        virtual uint32 GetHeight() const override { return m_Height; }

        virtual void Bind(uint32 slot) const override;

    private:
        String m_Path;
        uint32 m_Width;
        uint32 m_Height;
        uint32 m_RendererID;
    };
}