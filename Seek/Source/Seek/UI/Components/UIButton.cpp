#include "SeekPCH.h"
#include "Seek/UI/Components/UIButton.h"

namespace Seek
{
    UIButton::UIButton() {}

    UIButton::~UIButton() {}

    void UIButton::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(
            SK_BIND_EVENT_FN(UIButton::OnMousePressedEvent));
    }

    void UIButton::OnUpdate(float deltaTime) {}

    bool UIButton::OnMousePressedEvent(MouseButtonPressedEvent& event)
    {
        /*if (event.GetX() >= m_Position.x &&
            event.GetX() <= m_Position.x + m_Size.x)
        {
            if (event.GetY() >= m_Position.y &&
                event.GetY() <= m_Position.y + m_Size.y)
            {
                printf("Inside button\n");
                m_Color = sf::Color::Red;
                return true;
            }
        }*/

        return false;
    }
}