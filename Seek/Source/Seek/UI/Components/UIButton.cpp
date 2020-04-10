#include "SeekPCH.h"
#include "Seek/UI/Components/UIButton.h"

#include "Seek/UI/Constraints/UIRelativeConstraint.h"
#include "Seek/UI/Constraints/UITextHeightConstraint.h"
#include "Seek/UI/Constraints/UICenterConstraint.h"
#include "Seek/UI/UIConstraints.h"

namespace Seek
{
    UIButton::UIButton(const String& text, const Ref<Font>& font)
    {
        m_Block = new UIBlock();
        m_Text = new UIText(text, font, 0.0f, TextAlignment::CENTER);
    }

    UIButton::~UIButton() {}

    void UIButton::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(
            SK_BIND_EVENT_FN(UIButton::OnMousePressedEvent));
    }

    void UIButton::OnInit()
    {
        glm::vec2 textSize = m_Text->GetFont()->GetTextSize(m_Text->GetText());

        UIConstraints* constraints = new UIConstraints();
        constraints->SetX(new UIRelativeConstraint(0.0f));
        constraints->SetY(new UIRelativeConstraint(0.0f));
        constraints->SetWidth(new UIRelativeConstraint(1.0f));
        constraints->SetHeight(new UIRelativeConstraint(1.0f));
        Add(m_Block, constraints);

        constraints = new UIConstraints();
        constraints->SetX(new UIRelativeConstraint(0.0f));
        constraints->SetY(new UICenterConstraint());
        constraints->SetWidth(new UIRelativeConstraint(1.0f));
        constraints->SetHeight(new UITextHeightConstraint());
        Add(m_Text, constraints);
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