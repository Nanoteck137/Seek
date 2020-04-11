#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/UI/Components/UIBlock.h"
#include "Seek/UI/Components/UIText.h"

#include "Seek/UI/UIComponent.h"

namespace Seek
{
    class UIButton : public UIComponent
    {
    public:
        using ActionHandler = std::function<void()>;

    private:
        enum class ButtonState
        {
            PRESSED,
            UNPRESSED
        };

    public:
        UIButton(const String& text, const Ref<Font>& font);
        ~UIButton();

    private:
        bool InsideButton(float32 x, float32 y);
        void OnAction();

    public:
        virtual void OnEvent(Event& event) override;

    private:
        bool OnMouseMoved(MouseMovedEvent& event);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
        bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);

        static void NoAction() {}

    protected:
        virtual void OnInit() override;
        virtual void OnUpdate(float deltaTime) override;

    public:
        inline void SetAction(ActionHandler action)
        {
            m_ActionHandler = action;
        }

    private:
        UIBlock* m_Block = nullptr;
        UIText* m_Text = nullptr;

        Ref<Transition> m_HoverTransition;

        ButtonState m_State = ButtonState::UNPRESSED;
        bool m_MouseOver = false;

        ActionHandler m_ActionHandler = &UIButton::NoAction;
    };
}