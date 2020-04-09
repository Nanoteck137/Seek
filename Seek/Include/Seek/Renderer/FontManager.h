#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Font.h"

#undef CreateFont

namespace Seek
{
    class FontManager
    {
    public:
        static void Init();
        static void Shutdown();

        static Ref<Font> CreateFont(const String& filename, float size);

    private:
        static void ResetState();
        static void GenerateBitmap(const String& filename, float size);
        static void CopyBitmapToTexture(const Ref<Texture2D>& texture);
        static void CreateGlyphs(FontGlyph* glyphs);
    };
}