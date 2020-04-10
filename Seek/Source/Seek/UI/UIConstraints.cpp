#include "SeekPCH.h"
#include "Seek/UI/UIConstraints.h"

namespace Seek
{
    UIConstraints::UIConstraints() {}

    UIConstraints::~UIConstraints()
    {
        if (m_XConstraint)
        {
            delete m_XConstraint;
            m_XConstraint = nullptr;
        }

        if (m_YConstraint)
        {
            delete m_YConstraint;
            m_YConstraint = nullptr;
        }

        if (m_WidthConstraint)
        {
            delete m_WidthConstraint;
            m_WidthConstraint = nullptr;
        }

        if (m_HeightConstraint)
        {
            delete m_HeightConstraint;
            m_HeightConstraint = nullptr;
        }
    }

    void UIConstraints::NotifyAdded(UIComponent* current, UIComponent* parent)
    {
        SK_CORE_ASSERT(m_XConstraint, "X Constraint has not been set");
        SK_CORE_ASSERT(m_YConstraint, "Y Constraint has not been set");
        SK_CORE_ASSERT(m_WidthConstraint, "Width Constraint has not been set");
        SK_CORE_ASSERT(m_HeightConstraint,
                       "Height Constraint has not been set");

        m_XConstraint->NotifyAdded(this, current, parent);
        m_YConstraint->NotifyAdded(this, current, parent);
        m_WidthConstraint->NotifyAdded(this, current, parent);
        m_HeightConstraint->NotifyAdded(this, current, parent);
    }

    void UIConstraints::SetX(UIConstraint* constraint)
    {
        SK_CORE_ASSERT(constraint, "Constraint can't be nullptr");
        m_XConstraint = constraint;
        m_XConstraint->SetAxis(true, true);
    }

    void UIConstraints::SetY(UIConstraint* constraint)
    {
        SK_CORE_ASSERT(constraint, "Constraint can't be nullptr");
        m_YConstraint = constraint;
        m_YConstraint->SetAxis(false, true);
    }

    void UIConstraints::SetWidth(UIConstraint* constraint)
    {
        SK_CORE_ASSERT(constraint, "Constraint can't be nullptr");
        m_WidthConstraint = constraint;
        m_WidthConstraint->SetAxis(true, false);
    }

    void UIConstraints::SetHeight(UIConstraint* constraint)
    {
        SK_CORE_ASSERT(constraint, "Constraint can't be nullptr");
        m_HeightConstraint = constraint;
        m_HeightConstraint->SetAxis(false, false);
    }
}