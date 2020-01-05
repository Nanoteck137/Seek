#include "SeekPCH.h"
#include "Seek/Renderer/Texture.h"

#include "Seek/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Seek
{
    Ref<Texture2D> Texture2D::Create(const String& path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                SK_CORE_ASSERT(false, "RendererAPI::None is not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLTexture2D>(path);
        }

        SK_CORE_ASSERT(false, "No RenderingAPI selected");
        return nullptr;
    }
}