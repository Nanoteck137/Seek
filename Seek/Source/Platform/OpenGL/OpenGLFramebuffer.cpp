#include "SeekPCH.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include "Seek/Debug/Instrumentor.h"

#include <glad/glad.h>

namespace Seek
{
    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferOptions& options)
        : m_Options(options)
    {
        Create();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        // TODO(patrik): Delete color attachment
        glDeleteFramebuffers(1, &m_RendererID);
    }

    void OpenGLFramebuffer::Create()
    {
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        /*glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glTextureStorage2D(m_ColorAttachment, 0, GL_RGBA8, m_Options.Width,
                           m_Options.Height);

        glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR);
        glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR);*/

        TextureParameters params(TextureFormat::RGBA, TextureFilter::LINEAR,
                                 TextureWrap::CLAMP_TO_EDGE);
        m_ColorAttachment = CreateRef<OpenGLTexture2D>(
            m_Options.Width, m_Options.Height, params);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               m_ColorAttachment->GetRendererID(), 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }

    void OpenGLFramebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
}