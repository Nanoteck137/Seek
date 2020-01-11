#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/RendererAPI.h"

namespace Seek
{
    class RenderCommand
    {
    public:
        static void Init();
        static void Shutdown();

        static inline void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        static inline void Clear() { s_RendererAPI->Clear(); }

        static inline void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }

        static inline void DrawIndexed(const Ref<VertexArray>& vertexArray,
                                       uint32 count)
        {
            s_RendererAPI->DrawIndexed(vertexArray, count);
        }

    private:
        static RendererAPI* s_RendererAPI;
    };
}