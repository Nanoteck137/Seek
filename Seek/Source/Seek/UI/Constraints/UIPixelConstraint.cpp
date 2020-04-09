#include "SeekPCH.h"
#include "Seek/UI/Constraints/UIPixelConstraint.h"

namespace Seek
{
    UIPixelConstraint::UIPixelConstraint(int value) : m_Value(value) {}

    UIPixelConstraint::~UIPixelConstraint() {}

    void UIPixelConstraint::CompleteSetup(UIConstraints* constraints) {}

    float UIPixelConstraint::GetRelativeValue()
    {
        UIComponent* parent = GetParent();

        float parentSizePixels =
            IsXAxis() ? parent->GetPixelWidth() : parent->GetPixelHeight();

        float value = m_Value / parentSizePixels;

        return value;
    }

    void UIPixelConstraint::SetPixelValue(int value) {}

    void UIPixelConstraint::SetRelativeValue(float value) {}
}