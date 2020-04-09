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

        float XAdvance;
    };

    struct FontData;

    class Font
    {
    public:
        Font(const String& filename);

        FontGlyph GetGlyphInfo(char c);

        inline const Ref<Texture2D>& GetTexture() const { return m_Texture; }

    private:
        String m_Filename;
        Ref<Texture2D> m_Texture;
        Buffer m_FileData;

        FontData* m_Data;
    };
}
