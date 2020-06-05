#include "SeekPCH.h"
#include "Seek/Renderer/SubTexture2D.h"

namespace Seek
{
    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture,
                               const glm::vec2& min, const glm::vec2& max,
                               bool flipHorizontal)
        : m_Texture(texture)
    {
        if (!flipHorizontal)
        {
            m_Coords[0] = {min.x, min.y};
            m_Coords[1] = {max.x, min.y};
            m_Coords[2] = {max.x, max.y};
            m_Coords[3] = {min.x, max.y};
        }
        else
        {
            // NOTE(patrik): Flip horizontally
            m_Coords[0] = {max.x, min.y};
            m_Coords[1] = {min.x, min.y};
            m_Coords[2] = {min.x, max.y};
            m_Coords[3] = {max.x, max.y};
        }
    }

    Ref<SubTexture2D> SubTexture2D::CreateFromCoords(
        const Ref<Texture2D>& texture, const glm::vec2& coord,
        const glm::vec2& cellSize, bool flipHorizontal,
        const glm::vec2& spriteSize)
    {

        glm::vec2 min = {(coord.x * cellSize.x) / texture->GetWidth(),
                         (coord.y * cellSize.y) / texture->GetHeight()};
        glm::vec2 max = {
            ((coord.x + spriteSize.x) * cellSize.x) / texture->GetWidth(),
            ((coord.y + spriteSize.y) * cellSize.y) / texture->GetHeight()};
        return CreateRef<SubTexture2D>(texture, min, max, flipHorizontal);
    }

}