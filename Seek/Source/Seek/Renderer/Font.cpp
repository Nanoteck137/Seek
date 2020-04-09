#include "SeekPCH.h"
#include "Seek/Renderer/Font.h"

#include "stb_truetype.h"

namespace Seek
{
    Font::Font(const Ref<Texture2D>& texture, FontGlyph* glyphData,
               char firstChar, char lastChar)
        : m_Texture(texture), m_Glyphs(glyphData), m_FirstChar(firstChar),
          m_LastChar(lastChar)
    {
    }

    FontGlyph Font::GetGlyphInfo(char c)
    {
        int32 index = c - ' ';

        FontGlyph glyph = m_Glyphs[index];
        return glyph;
    }
}
