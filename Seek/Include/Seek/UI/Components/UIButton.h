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
        UIButton(const String& text, const Ref<Font>& font);
        ~UIButton();

    public:
        virtual void OnEvent(Event& event) override;

    protected:
        virtual void OnInit() override;
        virtual void OnUpdate(float deltaTime) override;

    private:
        bool OnMousePressedEvent(MouseButtonPressedEvent& event);

    private:
        UIBlock* m_Block = nullptr;
        UIText* m_Text = nullptr;
    };
}