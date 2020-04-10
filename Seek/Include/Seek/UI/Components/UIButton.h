#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/UI/UIComponent.h"

namespace Seek
{
    class UIButton : public UIComponent
    {
    public:
        UIButton();
        ~UIButton();

    public:
        virtual void OnEvent(Event& event) override;

    protected:
        virtual void OnUpdate(float deltaTime) override;

    private:
        bool OnMousePressedEvent(MouseButtonPressedEvent& event);

    private:
    };
}