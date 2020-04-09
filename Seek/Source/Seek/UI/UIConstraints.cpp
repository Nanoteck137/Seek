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
        m_XConstraint->NotifyAdded(this, current, parent);
        m_YConstraint->NotifyAdded(this, current, parent);
        m_WidthConstraint->NotifyAdded(this, current, parent);
        m_HeightConstraint->NotifyAdded(this, current, parent);
    }

    void UIConstraints::SetX(UIConstraint* constraint)
    {
        m_XConstraint = constraint;
        m_XConstraint->SetAxis(true, true);
    }

    void UIConstraints::SetY(UIConstraint* constraint)
    {
        m_YConstraint = constraint;
        m_YConstraint->SetAxis(false, true);
    }

    void UIConstraints::SetWidth(UIConstraint* constraint)
    {
        m_WidthConstraint = constraint;
        m_WidthConstraint->SetAxis(true, false);
    }

    void UIConstraints::SetHeight(UIConstraint* constraint)
    {
        m_HeightConstraint = constraint;
        m_HeightConstraint->SetAxis(false, false);
    }
}