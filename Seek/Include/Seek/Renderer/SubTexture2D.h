#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Seek
{
    class SubTexture2D
    {
    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min,
                     const glm::vec2& max, bool flipHorizontal = false);

        const Ref<Texture2D>& GetTexture() const { return m_Texture; }
        const glm::vec2* GetCoords() const { return m_Coords; }

    public:
        static Ref<SubTexture2D>
        CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coord,
                         const glm::vec2& cellSize, bool flipHorizontal = false,
                         const glm::vec2& spriteSize = {1.0f, 1.0f});

    private:
        Ref<Texture2D> m_Texture;
        glm::vec2 m_Coords[4];
    };
}