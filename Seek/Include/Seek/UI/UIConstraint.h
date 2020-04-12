#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "UIComponent.h"

namespace Seek
{
    class UIConstraint
    {
    public:
        UIConstraint();
        ~UIConstraint();

        float GetParentPixelSize();

        void NotifyAdded(UIConstraints* constraints, UIComponent* current,
                         UIComponent* parent);

        virtual void CompleteSetup(UIConstraints* constraints) = 0;
        virtual float GetRelativeValue() = 0;
        virtual void SetPixelValue(int32 value) = 0;
        virtual void SetRelativeValue(float32 value) = 0;

        void SetAxis(bool xAxis, bool posValue);

        inline bool IsPosValue() const { return m_PosValue; }
        inline bool IsXAxis() const { return m_XAxis; }

        inline UIComponent* GetCurrent() const { return m_Current; }
        inline UIComponent* GetParent() const { return m_Parent; }

    private:
        bool m_XAxis = false;
        bool m_PosValue = false;

        UIComponent* m_Current = nullptr;
        UIComponent* m_Parent = nullptr;
    };
}