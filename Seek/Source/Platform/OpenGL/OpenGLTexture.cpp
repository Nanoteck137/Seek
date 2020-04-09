#include "SeekPCH.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Seek/Debug/Instrumentor.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Seek
{
    OpenGLTexture2D::OpenGLTexture2D(uint32 width, uint32 height,
                                     TextureParameters params)
        : m_Width(width), m_Height(height)
    {
        SK_PROFILE_FUNCTION();

        // TODO(patrik): Change this!??!?!?
        if (params.Format == TextureFormat::ALPHA)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        else
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }

        SK_CORE_ASSERT(m_InternalFormat && m_DataFormat,
                       "Format not supported");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width,
                           m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    OpenGLTexture2D::OpenGLTexture2D(const String& path) : m_Path(path)
    {
        SK_PROFILE_FUNCTION();

        // TODO(patrik): Change this!??!?!?
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        SK_CORE_ASSERT(data, "Failed to load image!");

        m_Width = width;
        m_Height = height;

        if (channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }

        SK_CORE_ASSERT(m_InternalFormat && m_DataFormat,
                       "Format not supported");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width,
                           m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height,
                            m_DataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        SK_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32 size)
    {
        SK_PROFILE_FUNCTION();

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height,
                            m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32 slot) const
    {
        SK_PROFILE_FUNCTION();

        glBindTextureUnit(slot, m_RendererID);
    }
}