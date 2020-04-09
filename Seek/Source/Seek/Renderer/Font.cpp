#include "SeekPCH.h"
#include "Seek/Renderer/Font.h"

#include "stb_truetype.h"

namespace Seek
{
    // TODO: Change this
    struct FontData
    {
        stbtt_packedchar Bakedchars[96];
    };

    Font::Font(const Ref<Texture2D>& texture, FontGlyph* glyphData,
               char firstChar, char lastChar)
        : m_Texture(texture), m_Glyphs(glyphData), m_FirstChar(firstChar),
          m_LastChar(lastChar)
    {
        /*
        // TODO: Temp
        m_Data = new FontData();

        // stbtt_fontinfo font;
        // stbtt_InitFont(&font, m_FileData.Data, 0);

*/
    }

    FontGlyph Font::GetGlyphInfo(char c)
    {
        int32 index = c - ' ';

        FontGlyph glyph = m_Glyphs[index];
        return glyph;
    }
}
