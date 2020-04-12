#include "SeekPCH.h"
#include "Seek/UI/Constraints/UIRelativeConstraint.h"

namespace Seek
{
    UIRelativeConstraint::UIRelativeConstraint(float32 value) : m_Value(value)
    {
    }

    UIRelativeConstraint::~UIRelativeConstraint() {}

    void UIRelativeConstraint::CompleteSetup(UIConstraints* constraints) {}

    float UIRelativeConstraint::GetRelativeValue() { return m_Value; }

    void UIRelativeConstraint::SetPixelValue(int32 value) {}

    void UIRelativeConstraint::SetRelativeValue(float32 value) {}
}