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
        m_Texture = Texture2D::Create(
            512, 512,
            TextureParameters(TextureFormat::RGBA, TextureFilter::NEAREST,
                              TextureWrap::CLAMP_TO_EDGE));

        m_FileData = FileSystem::ReadAllBuffer(filename);

        // TODO: Temp
        m_Data = new FontData();

        uint8 buffer[512 * 512];
        uint32* textureData = new uint32[512 * 512];

        // stbtt_fontinfo font;
        // stbtt_InitFont(&font, m_FileData.Data, 0);

        stbtt_pack_context packContext;
        stbtt_PackBegin(&packContext, buffer, 512, 512, 0, 1, nullptr);

        stbtt_PackFontRange(&packContext, (const uint8*)m_FileData.Data, 0, 40,
                            ' ', '~' - ' ', m_Data->Bakedchars);

        stbtt_PackEnd(&packContext);

        for (uint32 i = 0; i < 512 * 512; i++)
        {
            uint8 alpha = buffer[i];
            textureData[i] =
                (alpha << 24) | (0xff << 16) | (0xff << 8) | (0xff << 0);
        }

        m_Texture->SetData(textureData, 512 * 512 * 4);
    }

    FontGlyph Font::GetGlyphInfo(char c)
    {
        int32 index = c - ' ';
        stbtt_packedchar* characterData = m_Data->Bakedchars + index;

        float32 ipw = 1.0f / m_Texture->GetWidth();
        float32 iph = 1.0f / m_Texture->GetHeight();
        float32 u0 = characterData->x0 * ipw;
        float32 v0 = characterData->y0 * iph;
        float32 u1 = characterData->x1 * ipw;
        float32 v1 = characterData->y1 * iph;

        float32 width = characterData->xoff2 - characterData->xoff;
        float32 height = characterData->yoff2 - characterData->yoff;

        FontGlyph result = {};
        result.Size = glm::vec2(width, height);
        result.UVRect = glm::vec4(u0, v0, u1, v1);
        result.Offset = glm::vec2(characterData->xoff, characterData->yoff2);
        result.XAdvance = characterData->xadvance;

        return result;
    }
}
