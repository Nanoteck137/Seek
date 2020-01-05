#include "SeekPCH.h"
#include "Seek/Renderer/Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include "Seek/Renderer/Renderer.h"

namespace Seek
{
	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
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

	VertexBuffer* VertexBuffer::Create(void* vertices, uint32 size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: SK_CORE_ASSERT(false, "RendererAPI::None is not supported!") return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		SK_CORE_ASSERT(false, "No RenderingAPI selected");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32* indices, uint32 count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: SK_CORE_ASSERT(false, "RendererAPI::None is not supported!") return nullptr;
		case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}

		SK_CORE_ASSERT(false, "No RenderingAPI selected");
		return nullptr;
	}
}