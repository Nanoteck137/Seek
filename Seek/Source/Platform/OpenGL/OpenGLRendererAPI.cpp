#include "SeekPCH.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include "Seek/Debug/Instrumentor.h"

#include <glad/glad.h>

namespace Seek
{
    OpenGLRendererAPI::OpenGLRendererAPI() { SK_PROFILE_FUNCTION(); }

    OpenGLRendererAPI::~OpenGLRendererAPI() { SK_PROFILE_FUNCTION(); }

    void OpenGLRendererAPI::Init()
    {
        SK_PROFILE_FUNCTION();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_ALWAYS);
    }

    void OpenGLRendererAPI::Shutdown() { SK_PROFILE_FUNCTION(); }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(),
                       GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray,
                                        uint32 count)
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
}