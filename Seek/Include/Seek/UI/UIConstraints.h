#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "UIConstraint.h"

namespace Seek
{
    class UIConstraints
    {
    public:
        UIConstraints();
        ~UIConstraints();

        void NotifyAdded(UIComponent* current, UIComponent* parent);

    public:
        void SetX(UIConstraint* constraint);
        void SetY(UIConstraint* constraint);
        void SetWidth(UIConstraint* constraint);
        void SetHeight(UIConstraint* constraint);

        inline UIConstraint* GetX() const { return m_XConstraint; }
        inline UIConstraint* GetY() const { return m_YConstraint; }
        inline UIConstraint* GetWidth() const { return m_WidthConstraint; }
        inline UIConstraint* GetHeight() const { return m_HeightConstraint; }

    private:
        UIConstraint* m_XConstraint = nullptr;
        UIConstraint* m_YConstraint = nullptr;
        UIConstraint* m_WidthConstraint = nullptr;
        UIConstraint* m_HeightConstraint = nullptr;
    };
}