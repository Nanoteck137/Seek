#pragma once

#include "Seek/Renderer/Buffer.h"

namespace Seek
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(BufferUsage usage);
        ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void Resize(uint32 size) override;
        virtual void SetData(void* data, uint32 size) override;

        virtual void* Map() override;
        virtual void Unmap() override;

        virtual void SetLayout(const BufferLayout& layout) override
        {
            m_Layout = layout;
        }
        virtual inline const BufferLayout& GetLayout() const override
        {
            return m_Layout;
        }

        virtual BufferUsage GetUsage() const override { return m_Usage; }

    private:
        uint32 m_RendererID;
        uint32 m_Size;
        BufferLayout m_Layout;
        BufferUsage m_Usage;
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