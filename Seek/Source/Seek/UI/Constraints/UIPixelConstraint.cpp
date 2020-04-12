#include "SeekPCH.h"
#include "Seek/UI/Constraints/UIPixelConstraint.h"

#include "Seek/UI/UIComponent.h"

namespace Seek
{
    UIPixelConstraint::UIPixelConstraint(int32 value) : m_Value(value) {}

    UIPixelConstraint::~UIPixelConstraint() {}

    void UIPixelConstraint::CompleteSetup(UIConstraints* constraints) {}

    float UIPixelConstraint::GetRelativeValue()
    {
        UIComponent* parent = GetParent();

        glm::vec2 size = parent->GetSizeInPixels();
        float parentSizePixels = IsXAxis() ? size.x : size.y;

        float value = m_Value / parentSizePixels;

        return value;
    }

    void UIPixelConstraint::SetPixelValue(int32 value) {}

    void UIPixelConstraint::SetRelativeValue(float32 value) {}
}