#include "SeekPCH.h"
#include "Seek/UI/Constraints/UIRatioConstraint.h"

#include "Seek/UI/UIConstraints.h"

namespace Seek
{
    UIRatioConstraint::UIRatioConstraint(float32 aspectRatio)
        : m_AspectRatio(aspectRatio)
    {
    }

    UIRatioConstraint::~UIRatioConstraint() {}

    void UIRatioConstraint::CompleteSetup(UIConstraints* constraints)
    {
        if (IsPosValue())
        {
            m_OtherConstraint =
                IsXAxis() ? constraints->GetY() : constraints->GetX();
        }
        else
        {
            m_OtherConstraint =
                IsXAxis() ? constraints->GetHeight() : constraints->GetWidth();
        }
    }

    float UIRatioConstraint::GetRelativeValue()
    {
        float otherRelValue = m_OtherConstraint->GetRelativeValue();
        float relativeValue =
            IsXAxis() ? GetParent()->GetRelativeWidthCoords(otherRelValue)
                      : GetParent()->GetRelativeHeightCoords(otherRelValue);
        return relativeValue * m_AspectRatio;
    }

    void UIRatioConstraint::SetPixelValue(int32 value)
    {
        SK_CORE_ASSERT(false, "Can't set pixel value");
    }

    void UIRatioConstraint::SetRelativeValue(float32 value)
    {
        SK_CORE_ASSERT(false, "Can't set relative value");
    }
}