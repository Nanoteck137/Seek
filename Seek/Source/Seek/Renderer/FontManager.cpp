#include "SeekPCH.h"
#include "Seek/Renderer/FontManager.h"

#include "Seek/System/FileSystem.h"

#include <stb_truetype.h>

namespace Seek
{
    struct FontFileData
    {
        String Filename;
        Buffer Data;
    };

    struct FontManagerStorage
    {
        char FirstChar;
        char LastChar;
        uint64 NumChars;

        stbtt_packedchar* PackedCharBuffer;

        uint32 TextureWidth;
        uint32 TextureHeight;

        std::unordered_map<String, FontFileData*> Files;

        uint8* BitmapBuffer;
        uint32* TextureBuffer;
    };

    static FontManagerStorage* s_Data;

    void FontManager::Init()
    {
        SK_CORE_ASSERT(!s_Data, "FontManager already initialized");

        s_Data = new FontManagerStorage;

        s_Data->FirstChar = ' ';
        s_Data->LastChar = '~';
        s_Data->NumChars = s_Data->LastChar - s_Data->FirstChar;

        s_Data->PackedCharBuffer = new stbtt_packedchar[s_Data->NumChars];

        s_Data->TextureWidth = 512;
        s_Data->TextureHeight = 512;

        uint64 numBufferElements = s_Data->TextureWidth * s_Data->TextureHeight;
        s_Data->BitmapBuffer = new uint8[numBufferElements];
        s_Data->TextureBuffer = new uint32[numBufferElements];
    }

    void FontManager::Shutdown()
    {
        SK_CORE_ASSERT(s_Data, "FontManager already shutdown");

        if (s_Data->TextureBuffer)
        {
            delete[] s_Data->TextureBuffer;
            s_Data->TextureBuffer = nullptr;
        }

        if (s_Data->BitmapBuffer)
        {
            delete[] s_Data->BitmapBuffer;
            s_Data->BitmapBuffer = nullptr;
        }

        if (s_Data->PackedCharBuffer)
        {
            delete[] s_Data->PackedCharBuffer;
            s_Data->PackedCharBuffer = nullptr;
        }

        if (s_Data)
        {
            delete s_Data;
            s_Data = nullptr;
        }
    }

    Ref<Font> FontManager::CreateFont(const String& filename, float size)
    {
        ResetState();

        FontGlyph* glyphs = new FontGlyph[s_Data->NumChars];
        Ref<Texture2D> atlas = Texture2D::Create(
            512, 512,
            TextureParameters(TextureFormat::RGBA, TextureFilter::NEAREST,
                              TextureWrap::CLAMP_TO_EDGE));

        GenerateBitmap(filename, size);
        CopyBitmapToTexture(atlas);
        CreateGlyphs(glyphs);

        return CreateRef<Font>(atlas, glyphs, s_Data->FirstChar,
                               s_Data->LastChar);
    }

    void FontManager::ResetState()
    {
        memset(s_Data->PackedCharBuffer, 0,
               s_Data->NumChars * sizeof(stbtt_packedchar));
        memset(s_Data->BitmapBuffer, 0,
               s_Data->TextureWidth * s_Data->TextureHeight * sizeof(uint8));
        memset(s_Data->TextureBuffer, 0,
               s_Data->TextureWidth * s_Data->TextureHeight * sizeof(uint32));
    }

    void FontManager::GenerateBitmap(const String& filename, float size)
    {
        String name = FileSystem::GetPathFileName(filename);

        FontFileData* file = nullptr;
        if (s_Data->Files.find(name) == s_Data->Files.end())
        {
            // Open File
            Buffer fileData = FileSystem::ReadAllBuffer(filename);

            file = new FontFileData;
            file->Data = fileData;
            file->Filename = filename;

            s_Data->Files[name] = file;
        }
        else
        {
            // Reuse file
            file = s_Data->Files[name];
        }

        stbtt_pack_context packContext;
        stbtt_PackBegin(&packContext, s_Data->BitmapBuffer,
                        s_Data->TextureWidth, s_Data->TextureHeight, 0, 1,
                        nullptr);

        stbtt_PackFontRange(&packContext, (const uint8*)file->Data.Data, 0,
                            size, s_Data->FirstChar, s_Data->NumChars,
                            s_Data->PackedCharBuffer);

        stbtt_PackEnd(&packContext);
    }

    void FontManager::CopyBitmapToTexture(const Ref<Texture2D>& texture)
    {
        for (uint32 i = 0; i < s_Data->TextureWidth * s_Data->TextureHeight;
             i++)
        {
            uint8 alpha = s_Data->BitmapBuffer[i];
            s_Data->TextureBuffer[i] =
                (alpha << 24) | (0xff << 16) | (0xff << 8) | (0xff << 0);
        }

        texture->SetData(s_Data->TextureBuffer, 512 * 512 * 4);
    }

    void FontManager::CreateGlyphs(FontGlyph* glyphs)
    {
        for (uint32 i = 0; i < s_Data->NumChars; i++)
        {
            stbtt_packedchar* characterData = s_Data->PackedCharBuffer + i;

            float32 ipw = 1.0f / s_Data->TextureWidth;
            float32 iph = 1.0f / s_Data->TextureHeight;
            float32 u0 = characterData->x0 * ipw;
            float32 v0 = characterData->y0 * iph;
            float32 u1 = characterData->x1 * ipw;
            float32 v1 = characterData->y1 * iph;

            float32 width = characterData->xoff2 - characterData->xoff;
            float32 height = characterData->yoff2 - characterData->yoff;

            FontGlyph glyph = {};
            glyph.Size = glm::vec2(width, height);
            glyph.UVRect = glm::vec4(u0, v0, u1, v1);
            glyph.Offset = glm::vec2(characterData->xoff, characterData->yoff2);
            glyph.XAdvance = characterData->xadvance;

            glyphs[i] = glyph;
        }
    }
}