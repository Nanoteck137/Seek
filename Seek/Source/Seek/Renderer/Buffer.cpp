#include "SeekPCH.h"
#include "Seek/Renderer/Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include "Seek/Renderer/Renderer.h"

namespace Seek
{
    BufferLayout::BufferLayout(
        const std::initializer_list<BufferElement>& elements)
        : m_Elements(elements)
    {
        CalculateOffsetsAndStride();
    }

    void BufferLayout::CalculateOffsetsAndStride()
    {
        uint32 offset = 0;
        m_Stride = 0;

        for (auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

    Ref<VertexBuffer> VertexBuffer::Create(BufferUsage usage)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                SK_CORE_ASSERT(false, "RendererAPI::None is not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLVertexBuffer>(usage);
        }

        SK_CORE_ASSERT(false, "No RenderingAPI selected");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32* indices, uint32 count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                SK_CORE_ASSERT(false, "RendererAPI::None is not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLIndexBuffer>(indices, count);
        }

        SK_CORE_ASSERT(false, "No RenderingAPI selected");
        return nullptr;
    }
}