#include "SeekPCH.h"
#include "Seek/UI/Constraints/UITextHeightConstraint.h"

namespace Seek
{
    UITextHeightConstraint::UITextHeightConstraint() {}

    UITextHeightConstraint::~UITextHeightConstraint() {}

    void UITextHeightConstraint::CompleteSetup(UIConstraints* constraints) {}

    float UITextHeightConstraint::GetRelativeValue()
    {
        return m_HeightPixels / GetParentPixelSize();
    }

    void UITextHeightConstraint::SetPixelValue(int value)
    {
        m_HeightPixels = value;
    }

    void UITextHeightConstraint::SetRelativeValue(float value)
    {
        m_HeightPixels = (int)(GetParentPixelSize() * value);
    }
}