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

        void SetX(UIConstraint* constraint);
        inline UIConstraint* GetX() const { return m_XConstraint; }

        void SetY(UIConstraint* constraint);
        inline UIConstraint* GetY() const { return m_YConstraint; }

        void SetWidth(UIConstraint* constraint);
        inline UIConstraint* GetWidth() const { return m_WidthConstraint; }

        void SetHeight(UIConstraint* constraint);
        inline UIConstraint* GetHeight() const { return m_HeightConstraint; }

    private:
        UIConstraint* m_XConstraint = nullptr;
        UIConstraint* m_YConstraint = nullptr;
        UIConstraint* m_WidthConstraint = nullptr;
        UIConstraint* m_HeightConstraint = nullptr;
    };
}