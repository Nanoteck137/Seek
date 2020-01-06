#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32 ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3: return 4 * 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4 * 4;
            case ShaderDataType::Int: return 4;
            case ShaderDataType::Int2: return 4 * 2;
            case ShaderDataType::Int3: return 4 * 3;
            case ShaderDataType::Int4: return 4 * 4;
            case ShaderDataType::Bool: return 1;
        }

        SK_CORE_ASSERT(false, "Unknown shader data type");
        return 0;
    }

    struct BufferElement
    {
        String Name;
        ShaderDataType Type;
        uint32 Offset;
        uint32 Size;
        bool Normalized;

        BufferElement() {}

        BufferElement(ShaderDataType type, const String& name,
                      bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0),
              Normalized(normalized)
        {
        }

        uint32 GetElementCount() const
        {
            switch (Type)
            {
                case ShaderDataType::Float: return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3: return 3 * 3;
                case ShaderDataType::Mat4: return 4 * 4;
                case ShaderDataType::Int: return 1;
                case ShaderDataType::Int2: return 2;
                case ShaderDataType::Int3: return 3;
                case ShaderDataType::Int4: return 4;
                case ShaderDataType::Bool: return 1;
            }

            SK_CORE_ASSERT(false, "Unknown shader data type");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() {}

        BufferLayout(const std::initializer_list<BufferElement>& elements);

        inline const std::vector<BufferElement>& GetElements() const
        {
            return m_Elements;
        }
        inline uint32 GetStride() const { return m_Stride; }

        std::vector<BufferElement>::iterator begin()
        {
            return m_Elements.begin();
        }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

    private:
        void CalculateOffsetsAndStride();

    private:
        std::vector<BufferElement> m_Elements;
        uint32 m_Stride = 0;
    };

    enum class BufferUsage
    {
        None = 0,
        Static,
        Dynamic
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void Resize(uint32 size) = 0;
        virtual void SetData(void* data, uint32 size) = 0;

        virtual void* Map() = 0;
        virtual void Unmap() = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        virtual BufferUsage GetUsage() const = 0;

    public:
        static Ref<VertexBuffer> Create(BufferUsage usage);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32 GetCount() const = 0;

    public:
        static Ref<IndexBuffer> Create(uint32* indices, uint32 count);
    };
}