#pragma once

#include "Seek/Renderer/Buffer.h"

namespace Seek
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* vertices, uint32 size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual inline const BufferLayout& GetLayout() const override { return m_Layout; }
	private:
		uint32 m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32* indices, uint32 count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual uint32 GetCount() const override { return m_Count; }
	private:
		uint32 m_Count;
		uint32 m_RendererID;
	};
}