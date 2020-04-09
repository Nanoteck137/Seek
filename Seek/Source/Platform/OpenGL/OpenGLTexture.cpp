#include "SeekPCH.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Seek/Debug/Instrumentor.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Seek
{
    static GLenum TextureFilterToOpenGL(TextureFilter filter)
    {
        switch (filter)
        {
            case TextureFilter::LINEAR: return GL_LINEAR;
            case TextureFilter::NEAREST: return GL_NEAREST;
        }

        SK_CORE_ASSERT(false, "Unknown Texture filter");
        return 0;
    }

    static GLenum TextureWrapToOpenGL(TextureWrap wrap)
    {
        switch (wrap)
        {
            case TextureWrap::REPEAT: return GL_REPEAT;
            case TextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
            case TextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
            case TextureWrap::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
        }

        SK_CORE_ASSERT(false, "Unknown texture wrap");
        return 0;
    }

    static GLenum TextureFormatToOpenGL(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RGB: return GL_RGB;
            case TextureFormat::RGBA: return GL_RGBA;
        }
        SK_CORE_ASSERT(false, "Unknown texture format");
        return 0;
    }

    static GLenum TextureFormatToOpenGLInternal(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RGB: return GL_RGB8;
            case TextureFormat::RGBA: return GL_RGBA8;
        }
        SK_CORE_ASSERT(false, "Unknown texture format");
        return 0;
    }

    OpenGLTexture2D::OpenGLTexture2D(uint32 width, uint32 height,
                                     TextureParameters params)
        : m_Width(width), m_Height(height), m_Parameters(params)
    {
        SK_PROFILE_FUNCTION();

        // TODO(patrik): Change this!??!?!?
        m_InternalFormat = TextureFormatToOpenGLInternal(m_Parameters.Format);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width,
                           m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER,
                            TextureFilterToOpenGL(m_Parameters.Filter));
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER,
                            TextureFilterToOpenGL(m_Parameters.Filter));

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            TextureWrapToOpenGL(m_Parameters.Wrap));
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            TextureWrapToOpenGL(m_Parameters.Wrap));
    }

    OpenGLTexture2D::OpenGLTexture2D(const String& path,
                                     TextureParameters params)
        : m_Path(path), m_Parameters(params)
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
            m_Parameters.Format = TextureFormat::RGBA;
        else if (channels == 3)
            m_Parameters.Format = TextureFormat::RGB;
        else
        {
            SK_CORE_ASSERT(false, "");
        }

        m_InternalFormat = TextureFormatToOpenGLInternal(m_Parameters.Format);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width,
                           m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER,
                            TextureFilterToOpenGL(m_Parameters.Filter));
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER,
                            TextureFilterToOpenGL(m_Parameters.Filter));

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            TextureWrapToOpenGL(m_Parameters.Wrap));
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            TextureWrapToOpenGL(m_Parameters.Wrap));

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height,
                            TextureFormatToOpenGL(m_Parameters.Format),
                            GL_UNSIGNED_BYTE, data);

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
                            TextureFormatToOpenGL(m_Parameters.Format),
                            GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32 slot) const
    {
        SK_PROFILE_FUNCTION();

        glBindTextureUnit(slot, m_RendererID);
    }
}