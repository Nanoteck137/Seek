#include "MouseController.h"

MouseController::MouseController(Seek::OrthographicCamera& camera,
                                 WorldController& worldController)
    : m_Camera(camera), m_WorldController(worldController)
{
}

void MouseController::Update(Seek::Timestep ts)
{
    glm::vec2 mousePos = Seek::Input::GetMousePosition();
    glm::vec2 worldPos = m_Camera.ConvertScreenToWorldPoint(mousePos);

    int32 tileX = floor(worldPos.x);
    int32 tileY = floor(worldPos.y);

    World& world = m_WorldController.GetWorld();

    if (tileX >= 0 && tileX < world.GetWidth())
    {
        if (tileY >= 0 && tileY < world.GetHeight())
        {
            Seek::Renderer2D::DrawQuad(glm::vec2(tileX, tileY),
                                       glm::vec2(1.0f, 1.0f),
                                       glm::vec4(1.0f, 0.0f, 1.0f, 0.5f));
        }
    }
}

void MouseController::OnEvent(Seek::Event& event)
{
    Seek::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Seek::MouseButtonReleasedEvent>(
        SK_BIND_EVENT_FN(MouseController::OnMouseButtonReleased));
}

bool MouseController::OnMouseButtonReleased(
    Seek::MouseButtonReleasedEvent& event)
{
    glm::vec2 mousePos = glm::vec2(event.GetX(), event.GetY());
    glm::vec2 worldPos = m_Camera.ConvertScreenToWorldPoint(mousePos);

    int32 tileX = floor(worldPos.x);
    int32 tileY = floor(worldPos.y);

    Tile* tile = m_WorldController.GetWorld().GetTileAt(tileX, tileY);
    if (tile)
    {
        if (tile->GetType() == Tile::Type::EMPTY)
            tile->SetType(Tile::Type::FLOOR);
        else if (tile->GetType() == Tile::Type::FLOOR)
            tile->SetType(Tile::Type::EMPTY);

        SK_APP_INFO("Clicking: {}, {}", tileX, tileY);
    }

    return false;
}
