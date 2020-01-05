#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/Buffer.h"

namespace Seek
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() {}

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void
        AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void
        AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>&
        GetVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

        static Ref<VertexArray> Create();
    };
}