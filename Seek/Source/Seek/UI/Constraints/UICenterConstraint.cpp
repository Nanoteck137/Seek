#include "SeekPCH.h"
#include "Seek/UI/Constraints/UICenterConstraint.h"

#include "Seek/UI/UIConstraints.h"

namespace Seek
{
    UICenterConstraint::UICenterConstraint() {}

    UICenterConstraint::~UICenterConstraint() {}

    void UICenterConstraint::CompleteSetup(UIConstraints* constraints)
    {
        m_SizeConstraint =
            IsXAxis() ? constraints->GetWidth() : constraints->GetHeight();
    }

    float UICenterConstraint::GetRelativeValue()
    {
        float size = m_SizeConstraint->GetRelativeValue();
        return (1 - size) / 2.0f;
    }

    void UICenterConstraint::SetPixelValue(int value) {}

    void UICenterConstraint::SetRelativeValue(float value) {}
}