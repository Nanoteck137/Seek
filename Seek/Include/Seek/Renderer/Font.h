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
        glm::vec2 Pos0;
        glm::vec2 Pos1;

        glm::vec2 TexCoord0;
        glm::vec2 TexCoord1;
    };

    struct FontData;

    class Font
    {
    public:
        Font(const String& filename);

        FontGlyph GetGlyphInfo(char c, float x, float y);

        inline const Ref<Texture2D>& GetTexture() const { return m_Texture; }

    private:
        String m_Filename;
        Ref<Texture2D> m_Texture;
        Buffer m_FileData;

        FontData* m_Data;
    };
}