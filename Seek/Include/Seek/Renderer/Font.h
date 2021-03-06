#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/Texture.h"
#include "Seek/System/FileSystem.h"

#include <glm/glm.hpp>

namespace Seek
{
    struct FontGlyph
    {
        glm::vec2 Size;
        glm::vec4 UVRect;

        glm::vec2 Offset;

        float32 XAdvance;
    };

    class Font
    {
    public:
        Font(const Ref<Texture2D>& texture, FontGlyph* glyphData,
             char firstChar, char lastChar);

        FontGlyph GetGlyphInfo(char c);

        float32 GetTextWidth(const String& text);
        float32 GetTextHeight(const String& text);
        glm::vec2 GetTextSize(const String& text);

    public:
        inline const Ref<Texture2D>& GetTexture() const { return m_Texture; }

    private:
        Ref<Texture2D> m_Texture;
        FontGlyph* m_Glyphs;

        char m_FirstChar;
        char m_LastChar;
    };
}
