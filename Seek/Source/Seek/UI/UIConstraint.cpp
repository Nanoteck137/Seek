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
        glm::vec2 size = m_Parent->GetSizeInPixels();
        return m_XAxis ? size.x : size.y;
    }

    void UIConstraint::SetAxis(bool xAxis, bool posValue)
    {
        m_XAxis = xAxis;
        m_PosValue = posValue;
    }
}