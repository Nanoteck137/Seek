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

    public:
        virtual void OnEvent(Event& event) override;

    private:
        bool OnMouseMoved(MouseMovedEvent& event);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
        bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);

    protected:
        virtual void OnInit() override;
        virtual void OnUpdate(float deltaTime) override;

    private:
        UIBlock* m_Block = nullptr;
        UIText* m_Text = nullptr;

        Ref<Transition> m_HoverTransition;

        bool m_MouseOver = false;

        ButtonState m_State = ButtonState::UNPRESSED;
    };
}