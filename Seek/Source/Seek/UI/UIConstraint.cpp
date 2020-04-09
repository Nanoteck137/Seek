#include "SeekPCH.h"
#include "Seek/UI/UIConstraint.h"

namespace Seek
{
    UIConstraint::UIConstraint() {}

    UIConstraint::~UIConstraint()
    {
        m_Current = nullptr;
        m_Parent = nullptr;
    }

    void UIConstraint::NotifyAdded(UIConstraints* constraints,
                                   UIComponent* current, UIComponent* parent)
    {
        m_Current = current;
        m_Parent = parent;
        CompleteSetup(constraints);
    }

    float UIConstraint::GetParentPixelSize()
    {
        return m_XAxis ? m_Parent->GetPixelWidth() : m_Parent->GetPixelHeight();
    }

    void UIConstraint::SetAxis(bool xAxis, bool posValue)
    {
        m_XAxis = xAxis;
        m_PosValue = posValue;
    }
}