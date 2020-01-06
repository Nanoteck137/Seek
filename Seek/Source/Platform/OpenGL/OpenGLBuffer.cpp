#include "SeekPCH.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Seek
{
    static GLenum BufferUsageToOpenGL(BufferUsage usage)
    {
        switch (usage)
        {
            case BufferUsage::Static: return GL_STATIC_DRAW;
            case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
        }

        SK_CORE_ASSERT(false, "Unknown buffer usage");
        return 0;
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(BufferUsage usage)
        : m_Size(0), m_Usage(usage)
    {
        glGenBuffers(1, &m_RendererID);
        /*glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);*/
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::Resize(uint32 size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr,
                     BufferUsageToOpenGL(m_Usage));
    }

    void OpenGLVertexBuffer::SetData(void* data, uint32 size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        // glBufferData(GL_ARRAY_BUFFER, );
    }

    void* OpenGLVertexBuffer::Map()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    }

    void OpenGLVertexBuffer::Unmap()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32* indices, uint32 count)
        : m_Count(count)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32), indices,
                     GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
}