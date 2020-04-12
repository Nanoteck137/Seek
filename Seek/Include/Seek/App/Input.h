#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/App/KeyCodes.h"
#include "Seek/App/MouseButtonCodes.h"

namespace Seek
{
    class Input
    {
    public:
        inline static bool IsKeyPressed(int keycode)
        {
            return s_Instance->IsKeyPressedImpl(keycode);
        }

        inline static bool IsMouseButtonPressed(int button)
        {
            return s_Instance->IsMouseButtonPressedImpl(button);
        }
        inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;

        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

    private:
        static Input* s_Instance;
    };
}