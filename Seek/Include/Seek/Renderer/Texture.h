#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    enum class TextureFormat
    {
        NONE = 0,
        RGB,
        RGBA,
        ALPHA
    };

    struct TextureParameters
    {
        TextureFormat Format;
    };

    class Texture
    {
    public:
        virtual ~Texture() {}

        virtual uint32 GetWidth() const = 0;
        virtual uint32 GetHeight() const = 0;

        virtual void SetData(void* data, uint32 size) = 0;

        virtual void Bind(uint32 slot = 0) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        virtual ~Texture2D() {}

        static Ref<Texture2D> Create(uint32 width, uint32 height,
                                     TextureParameters params);
        static Ref<Texture2D> Create(const String& path);
    };
}