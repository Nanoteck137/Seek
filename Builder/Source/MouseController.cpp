#include "MouseController.h"

MouseController::MouseController(WorldController& worldController)
    : m_WorldController(worldController)
{
}

void MouseController::Update(Seek::Timestep ts)
{
    glm::vec2 mousePos = Seek::Input::GetMousePosition();

    int32 tileX = floor((mousePos.x / 32.0f));
    int32 tileY = floor((mousePos.y / 32.0f));

    World& world = m_WorldController.GetWorld();

    if (tileX >= 0 && tileX < world.GetWidth())
    {
        if (tileY >= 0 && tileY < world.GetHeight())
        {
            Seek::Renderer2D::DrawQuad(glm::vec2(tileX * 32.0f, tileY * 32.0f),
                                       glm::vec2(32.0f, 32.0f),
                                       glm::vec4(1.0f, 0.0f, 1.0f, 0.5f));
        }
    }

    SK_APP_INFO("Tile: {}, {}", tileX, tileY);
}
