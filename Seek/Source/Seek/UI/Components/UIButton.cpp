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
    UIButton::UIButton(const Properties& props) : m_Props(props) {}

    UIButton::~UIButton() {}

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
        m_Block = new UIBlock();
        m_Block->SetColor(m_Props.Color);
        m_Block->SetBorderActive(m_Props.Border);
        m_Block->SetBorderThickness(m_Props.BorderThickness);
        m_Block->SetBorderColor(m_Props.BorderColor);

        UIConstraints* constraints = new UIConstraints();
        constraints->SetX(new UIRelativeConstraint(0.0f));
        constraints->SetY(new UIRelativeConstraint(0.0f));
        constraints->SetWidth(new UIRelativeConstraint(1.0f));
        constraints->SetHeight(new UIRelativeConstraint(1.0f));
        Add(m_Block, constraints);

        m_Text = new UIText(m_Props.Label, m_Props.Font, 0.0f,
                            TextAlignment::CENTER);
        m_Text->SetColor(m_Props.LabelColor);

        constraints = new UIConstraints();
        constraints->SetX(new UIRelativeConstraint(0.0f));
        constraints->SetY(new UICenterConstraint());
        constraints->SetWidth(new UIRelativeConstraint(1.0f));
        constraints->SetHeight(new UITextHeightConstraint());
        Add(m_Text, constraints);

        m_HoverBlockTransition = CreateRef<Transition>();
        m_HoverBlockTransition->Add(TransitionType::XPOS,
                                    new SlideTransition(0.05f, 0.10f));

        m_HoverTextTransition = CreateRef<Transition>();
        m_HoverTextTransition->Add(TransitionType::XPOS,
                                   new SlideTransition(0.02f, 0.15f));
    }

    void UIButton::OnUpdate(Timestep ts) {}

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

    bool UIButton::OnMouseMoved(MouseMovedEvent& event)
    {
        float32 mouseX = event.GetX() / UIManager::GetDisplayWidth();
        float32 mouseY = event.GetY() / UIManager::GetDisplayHeight();

        if (InsideButton(mouseX, mouseY))
        {
            if (m_MouseOver == false)
            {
                m_Block->GetAnimator()->ApplyModifier(m_HoverBlockTransition,
                                                      false, 0.0f);
                m_Text->GetAnimator()->ApplyModifier(m_HoverTextTransition,
                                                     false, 0.0f);
                m_Block->SetColor(m_Props.HoverColor);
            }

            m_MouseOver = true;

            return true;
        }
        else
        {
            if (m_MouseOver == true)
            {
                m_Block->GetAnimator()->ApplyModifier(m_HoverBlockTransition,
                                                      true, 0.0f);
                m_Text->GetAnimator()->ApplyModifier(m_HoverTextTransition,
                                                     true, 0.0f);
                m_Block->SetColor(m_Props.Color);
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
            m_Block->SetColor(m_Props.ClickColor);
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
            m_Block->SetColor(m_Props.HoverColor);
            OnAction();
        }
        else
        {
            m_Block->SetColor(m_Props.Color);
        }

        m_State = ButtonState::UNPRESSED;

        return false;
    }
}