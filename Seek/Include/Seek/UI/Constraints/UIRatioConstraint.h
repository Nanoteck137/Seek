#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/UI/UIConstraint.h"

namespace Seek
{
    class UIRatioConstraint : public UIConstraint
    {
    public:
        UIRatioConstraint(float aspectRatio);
        ~UIRatioConstraint();

    public:
        virtual void CompleteSetup(UIConstraints* constraints) override;
        virtual float GetRelativeValue() override;
        virtual void SetPixelValue(int value) override;
        virtual void SetRelativeValue(float value) override;

    private:
        float m_AspectRatio = 0.0f;
        UIConstraint* m_OtherConstraint = nullptr;
    };
}