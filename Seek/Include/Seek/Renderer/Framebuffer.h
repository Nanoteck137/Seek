#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Texture.h"

namespace Seek
{
    struct FramebufferOptions
    {
        uint32 Width;
        uint32 Height;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual const FramebufferOptions& GetOptions() const = 0;

        virtual Ref<Texture2D> GetColorAttachmentTexture() const = 0;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

    public:
        static Ref<Framebuffer> Create(const FramebufferOptions& options);
    };
}