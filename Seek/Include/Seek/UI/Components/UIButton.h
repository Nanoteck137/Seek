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
        struct Properties
        {
            String Label = "";
            Ref<Font> Font = nullptr;
            glm::vec4 LabelColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            glm::vec4 HoverColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
            glm::vec4 ClickColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

            bool Border = false;
            float BorderThickness = 0.0f;
            glm::vec4 BorderColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
        };

        using ActionHandler = std::function<void()>;

    private:
        enum class ButtonState
        {
            PRESSED,
            UNPRESSED
        };

    public:
        UIButton(const Properties& props);
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

        Properties m_Props;

        Ref<Transition> m_HoverBlockTransition;
        Ref<Transition> m_HoverTextTransition;

        ButtonState m_State = ButtonState::UNPRESSED;
        bool m_MouseOver = false;

        ActionHandler m_ActionHandler = &UIButton::NoAction;
    };
}