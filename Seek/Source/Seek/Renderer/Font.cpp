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

    float32 Font::GetTextWidth(const String& text)
    {
        float32 width = 0.0f;

        for (uint32 i = 0; i < text.size(); i++)
        {
            char c = text[i];
            FontGlyph info = GetGlyphInfo(c);

            width += info.XAdvance;
        }

        return width;
    }

    float32 Font::GetTextHeight(const String& text)
    {
        float32 min = 0.0f;
        float32 max = 0.0f;

        for (uint32 i = 0; i < text.size(); i++)
        {
            char c = text[i];
            FontGlyph info = GetGlyphInfo(c);

            float32 height = info.Size.y;
            float32 offset = info.Offset.y + height;

            if (offset < min)
                min = offset;
            if (height > max)
                max = height;
        }

        return abs(min) + abs(max);
    }

    glm::vec2 Font::GetTextSize(const String& text)
    {
        return glm::vec2(GetTextWidth(text), GetTextHeight(text));
    }
}
