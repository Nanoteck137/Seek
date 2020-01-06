#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Seek
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0,
            OpenGL = 1,
        };

    public:
        virtual ~RendererAPI() {}

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray,
                                 uint32 count) = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

        static inline API GetAPI() { return s_API; }

    private:
        static API s_API;
    };
}