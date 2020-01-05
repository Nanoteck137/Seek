#include "SeekPCH.h"
#include "Seek/Renderer/VertexArray.h"

#include "Seek/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Seek
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None:
                SK_CORE_ASSERT(false, "RendererAPI::None is not supported!")
                return nullptr;
            case RendererAPI::OpenGL: return CreateRef<OpenGLVertexArray>();
        }

        SK_CORE_ASSERT(false, "No RenderingAPI selected");
        return nullptr;
    }
}