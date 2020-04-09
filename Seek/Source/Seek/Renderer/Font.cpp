#include "SeekPCH.h"
#include "Seek/Renderer/Font.h"

#include "stb_truetype.h"

namespace Seek
{
    struct FontData
    {
        stbtt_bakedchar Bakedchars[96];
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
        stbtt_BakeFontBitmap((const uint8*)m_FileData.Data, 0, 32.0f, buffer,
                             512, 512, 32, 96, m_Data->Bakedchars);

        m_Texture->SetData(buffer, 512 * 512);
    }

    FontGlyph Font::GetGlyphInfo(char c, float x, float y)
    {
        FontGlyph result;

        float xPos = 64;
        float yPos = 32;

        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(m_Data->Bakedchars, 512, 512, c, &xPos, &yPos, &quad,
                           1);

        result.Pos0 = glm::vec2(quad.x0, quad.y0);
        result.Pos1 = glm::vec2(quad.x1, quad.y1);
        result.TexCoord0 = glm::vec2(quad.s0, quad.t0);
        result.TexCoord1 = glm::vec2(quad.s1, quad.t1);

        return result;
    }
}
