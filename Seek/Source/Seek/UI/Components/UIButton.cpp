#include "SeekPCH.h"
#include "Seek/UI/Components/UIButton.h"

#include "Seek/UI/Constraints/UIRelativeConstraint.h"
#include "Seek/UI/Constraints/UITextHeightConstraint.h"
#include "Seek/UI/Constraints/UICenterConstraint.h"
#include "Seek/UI/UIConstraints.h"

#include "Seek/UI/Transition/SlideTransition.h"

#include "Seek/UI/UIManager.h"

namespace Seek
{
    UIButton::UIButton(const String& text, const Ref<Font>& font)
    {
        m_Block = new UIBlock();
        m_Text = new UIText(text, font, 0.0f, TextAlignment::CENTER);
    }

    UIButton::~UIButton() {}

    bool UIButton::InsideButton(float32 x, float32 y)
    {
        if (x >= m_Position.x && x <= m_Position.x + m_Size.x)
        {
            if (y >= m_Position.y && y <= m_Position.y + m_Size.y)
            {
                return true;
            }
        }

        return false;
    }

    void UIButton::OnAction()
    {
        if (m_ActionHandler)
            m_ActionHandler();
    }

    void UIButton::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseMovedEvent>(
            SK_BIND_EVENT_FN(UIButton::OnMouseMoved));
        dispatcher.Dispatch<MouseButtonPressedEvent>(
            SK_BIND_EVENT_FN(UIButton::OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(
            SK_BIND_EVENT_FN(UIButton::OnMouseButtonReleased));
    }

    void UIButton::OnInit()
    {
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

        m_HoverTransition = CreateRef<Transition>();
        m_HoverTransition->Add(TransitionType::XPOS,
                               new SlideTransition(0.05f, 0.10f));
    }

    void UIButton::OnUpdate(float deltaTime)
    {
        if (m_MouseOver)
        {
        }
        else
        {
        }
    }

    bool UIButton::OnMouseMoved(MouseMovedEvent& event)
    {
        float32 mouseX = event.GetX() / UIManager::GetDisplayWidth();
        float32 mouseY = event.GetY() / UIManager::GetDisplayHeight();

        if (InsideButton(mouseX, mouseY))
        {
            if (m_MouseOver == false)
            {
                m_Block->GetAnimator()->ApplyModifier(m_HoverTransition, false,
                                                      0.0f);
                m_Block->SetColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
            }

            m_MouseOver = true;
        }
        else
        {
            if (m_MouseOver == true)
            {
                m_Block->GetAnimator()->ApplyModifier(m_HoverTransition, true,
                                                      0.0f);
                m_Block->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            }
            m_MouseOver = false;
        }

        return false;
    }

    bool UIButton::OnMouseButtonPressed(MouseButtonPressedEvent& event)
    {
        float32 mouseX = event.GetX() / UIManager::GetDisplayWidth();
        float32 mouseY = event.GetY() / UIManager::GetDisplayHeight();

        if (InsideButton(mouseX, mouseY))
        {
            m_State = ButtonState::PRESSED;
            return true;
        }

        return false;
    }

    bool UIButton::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
    {
        float32 mouseX = event.GetX() / UIManager::GetDisplayWidth();
        float32 mouseY = event.GetY() / UIManager::GetDisplayHeight();

        if (InsideButton(mouseX, mouseY) && m_State == ButtonState::PRESSED)
        {
            OnAction();
        }

        m_State = ButtonState::UNPRESSED;

        return false;
    }
}