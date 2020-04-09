#include "SeekPCH.h"
#include "Seek/Renderer/Font.h"

#include "stb_truetype.h"

namespace Seek
{
    struct FontData
    {
        stbtt_packedchar Bakedchars[96];
    };

    Font::Font(const String& filename) : m_Filename(filename)
    {
        TextureParameters params = {};
        params.Format = TextureFormat::ALPHA;
        m_Texture = Texture2D::Create(512, 512, params);

        m_FileData = FileSystem::ReadAllBuffer(filename);

        // TODO: Temp
        m_Data = new FontData();

        uint8 buffer[512 * 512];
        uint32* textureData = new uint32[512 * 512];

        // stbtt_fontinfo font;
        // stbtt_InitFont(&font, m_FileData.Data, 0);

        stbtt_pack_context packContext;
        stbtt_PackBegin(&packContext, buffer, 512, 512, 0, 1, nullptr);

        stbtt_PackFontRange(&packContext, (const uint8*)m_FileData.Data, 0, 32,
                            ' ', '~' - ' ', m_Data->Bakedchars);

        stbtt_PackEnd(&packContext);

        for (int i = 0; i < 512 * 512; i++)
        {
            uint8 alpha = buffer[i];
            textureData[i] =
                (alpha << 24) | (0xff << 16) | (0xff << 8) | (0xff << 0);
        }

        m_Texture->SetData(textureData, 512 * 512 * 4);
    }

    FontGlyph Font::GetGlyphInfo(char c, float x, float y)
    {
        FontGlyph result;

        stbtt_aligned_quad quad;
        /*stbtt_GetBakedQuad(m_Data->Bakedchars, 512, 512, c - 32, &x, &y,
           &quad, 1);*/

        stbtt_GetPackedQuad(m_Data->Bakedchars, 512, 512, c - ' ', &x, &y,
                            &quad, 1);

        result.Pos0 = glm::vec2(quad.x0, -quad.y1);
        result.Pos1 = glm::vec2(quad.x1, -quad.y0);
        result.TexCoord0 = glm::vec2(quad.s0, quad.t0);
        result.TexCoord1 = glm::vec2(quad.s1, quad.t1);
        result.Offset = glm::vec2(x, y);

        return result;
    }
}
