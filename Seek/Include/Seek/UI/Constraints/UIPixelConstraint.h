#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/UI/UIConstraint.h"

namespace Seek
{
    class UIPixelConstraint : public UIConstraint
    {
    public:
        UIPixelConstraint(int32 value);
        ~UIPixelConstraint();

    public:
        virtual void CompleteSetup(UIConstraints* constraints) override;
        virtual float GetRelativeValue() override;
        virtual void SetPixelValue(int32 value) override;
        virtual void SetRelativeValue(float32 value) override;

    private:
        int32 m_Value = 0;
    };
}