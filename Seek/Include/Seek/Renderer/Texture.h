#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    enum class TextureWrap
    {
        NONE = 0,
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };

    enum class TextureFilter
    {
        NONE = 0,
        LINEAR,
        NEAREST
    };

    enum class TextureFormat
    {
        NONE = 0,
        RGB,
        RGBA,
    };

    struct TextureParameters
    {
        TextureFormat Format;
        TextureFilter Filter;
        TextureWrap Wrap;

        TextureParameters()
        {
            Format = TextureFormat::RGBA;
            Filter = TextureFilter::NEAREST;
            Wrap = TextureWrap::CLAMP_TO_EDGE;
        }

        TextureParameters(TextureFormat format, TextureFilter filter,
                          TextureWrap wrap)
            : Format(format), Filter(filter), Wrap(wrap)
        {
        }

        TextureParameters(TextureFilter filter)
            : Format(TextureFormat::RGBA), Filter(filter),
              Wrap(TextureWrap::CLAMP_TO_EDGE)
        {
        }

        TextureParameters(TextureFilter filter, TextureWrap wrap)
            : Format(TextureFormat::RGBA), Filter(filter), Wrap(wrap)
        {
        }
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

        static Ref<Texture2D> Create(const String& path,
                                     TextureParameters params);
        static Ref<Texture2D> Create(const String& path);
    };
}