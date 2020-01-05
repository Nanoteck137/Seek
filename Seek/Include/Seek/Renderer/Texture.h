#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    class Texture
    {
    public:
        virtual ~Texture() {}

        virtual uint32 GetWidth() const = 0;
        virtual uint32 GetHeight() const = 0;

        virtual void Bind(uint32 slot = 0) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        virtual ~Texture2D() {}

        static Ref<Texture2D> Create(const String& path);
    };
}