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

    void UITextHeightConstraint::SetPixelValue(int32 value)
    {
        m_HeightPixels = value;
    }

    void UITextHeightConstraint::SetRelativeValue(float32 value)
    {
        m_HeightPixels = (int32)(GetParentPixelSize() * value);
    }
}