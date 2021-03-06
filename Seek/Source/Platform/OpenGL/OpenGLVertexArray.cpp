#include "SeekPCH.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Seek/Debug/Instrumentor.h"

#include <glad/glad.h>

namespace Seek
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3: return GL_FLOAT;
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
        }

        SK_CORE_ASSERT(false, "Unknown shader data type");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        SK_PROFILE_FUNCTION();

        glGenVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        SK_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind()
    {
        SK_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind()
    {
        SK_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(
        const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        SK_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32 index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        const auto& elements = layout.GetElements();
        for (const auto& element : elements)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.GetElementCount(),
                                  ShaderDataTypeToOpenGLBaseType(element.Type),
                                  element.Normalized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(),
                                  (const void*)element.Offset);
            index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::AddIndexBuffer(
        const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        SK_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}