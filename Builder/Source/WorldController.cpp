#include "WorldController.h"

WorldController::WorldController() { m_World = new World(10, 10); }

void WorldController::Update(Seek::Timestep ts)
{
    const float TILE_WIDTH = 32.0f;
    const float TILE_HEIGHT = 32.0f;

    for (uint32 y = 0; y < m_World->GetHeight(); y++)
    {
        for (uint32 x = 0; x < m_World->GetWidth(); x++)
        {
            Tile& tile = m_World->GetTileAt(x, y);

            glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            if (tile.GetType() == Tile::Type::FLOOR)
                color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            Seek::Renderer2D::DrawQuad(
                glm::vec2(0.1f + x * TILE_WIDTH, 0.1f + y * TILE_WIDTH),
                glm::vec2(TILE_WIDTH, TILE_HEIGHT), color);
        }
    }
}
