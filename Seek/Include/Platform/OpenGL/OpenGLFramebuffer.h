#pragma once

#include "Seek/Renderer/Framebuffer.h"

#include "OpenGLTexture.h"

namespace Seek
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferOptions& options);
        ~OpenGLFramebuffer();

        virtual const FramebufferOptions& GetOptions() const override
        {
            return m_Options;
        }

        virtual Ref<Texture2D> GetColorAttachmentTexture() const override
        {
            return m_ColorAttachment;
        }

        virtual void Bind() override;
        virtual void Unbind() override;

    private:
        void Create();

    private:
        FramebufferOptions m_Options = {};

        uint32 m_RendererID = 0;
        Ref<OpenGLTexture2D> m_ColorAttachment;
        // uint32 m_ColorAttachment;
        uint32 m_DepthAttachment = 0;
    };
}