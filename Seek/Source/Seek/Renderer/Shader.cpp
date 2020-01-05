#include "SeekPCH.h"
#include "Seek/Renderer/Shader.h"

#include "Seek/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Seek
{
    Ref<Shader> Shader::Create(const String& vertexSource,
                               const String& fragmentSource)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None:
                SK_CORE_ASSERT(false, "RendererAPI::None is not supported!")
                return nullptr;
            case RendererAPI::OpenGL:
                return CreateRef<OpenGLShader>(vertexSource, fragmentSource);
        }

        SK_CORE_ASSERT(false, "No RenderingAPI selected");
        return nullptr;
    }
}