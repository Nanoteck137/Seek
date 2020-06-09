#include "SeekPCH.h"
#include "Seek/Renderer/Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include "Seek/Renderer/Renderer.h"

namespace Seek
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferOptions& options)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                SK_CORE_ASSERT(false, "RendererAPI::None is not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLFramebuffer>(options);
        }

        SK_CORE_ASSERT(false, "No RenderingAPI selected");
        return nullptr;
    }
}