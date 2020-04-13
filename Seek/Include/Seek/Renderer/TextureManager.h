#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/Texture.h"

namespace Seek
{
    class TextureManager
    {
    public:
        static void Init();
        static void Shutdown();

        static Ref<Texture2D>
        LoadTexture(const String& name, const String& filename,
                    TextureParameters params = TextureParameters());
        static Ref<Texture2D> GetTexture(const String& name);
    };
}