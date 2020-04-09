#pragma once

#include "Seek/Renderer/Texture.h"

namespace Seek
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32 width, uint32 height, TextureParameters params);
        OpenGLTexture2D(const String& path, TextureParameters params);
        ~OpenGLTexture2D();

        virtual uint32 GetWidth() const override { return m_Width; }
        virtual uint32 GetHeight() const override { return m_Height; }

        virtual void SetData(void* data, uint32 size) override;

        virtual void Bind(uint32 slot) const override;

    private:
        String m_Path;
        uint32 m_RendererID = 0;
        uint32 m_Width = 0, m_Height = 0;
        uint32 m_InternalFormat = 0;
        TextureParameters m_Parameters;
    };
}