#include "WorldController.h"

WorldController::WorldController() { m_World = new World(10, 10); }

void WorldController::Update(Seek::Timestep ts)
{
    for (uint32 y = 0; y < m_World->GetHeight(); y++)
    {
        for (uint32 x = 0; x < m_World->GetWidth(); x++)
        {
            Tile& tile = m_World->GetTileAt(x, y);

            glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            if (tile.GetType() == Tile::Type::FLOOR)
                color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            Seek::Renderer2D::DrawQuad(glm::vec2(x, y), glm::vec2(1.0f, 1.0f),
                                       color);
        }
    }
}
