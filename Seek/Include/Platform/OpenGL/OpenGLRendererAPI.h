#pragma once

#include "Seek/Renderer/RendererAPI.h"

namespace Seek
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        OpenGLRendererAPI();
        ~OpenGLRendererAPI();

        virtual void UpdateDisplaySize(int32 width, int32 height) override;

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray,
                                 uint32 count) override;
    };
}