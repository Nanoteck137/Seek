#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/UI/UIConstraint.h"

namespace Seek
{
    class UIPixelConstraint : public UIConstraint
    {
    public:
        UIPixelConstraint(int value);
        ~UIPixelConstraint();

    public:
        virtual void CompleteSetup(UIConstraints* constraints) override;
        virtual float GetRelativeValue() override;
        virtual void SetPixelValue(int value) override;
        virtual void SetRelativeValue(float value) override;

    private:
        int m_Value = 0;
    };
}