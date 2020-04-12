#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/UI/UIConstraint.h"

namespace Seek
{
    class UIRelativeConstraint : public UIConstraint
    {
    public:
        UIRelativeConstraint(float32 value);
        ~UIRelativeConstraint();

    public:
        virtual void CompleteSetup(UIConstraints* constraints) override;
        virtual float GetRelativeValue() override;
        virtual void SetPixelValue(int32 value) override;
        virtual void SetRelativeValue(float32 value) override;

    private:
        float32 m_Value;
    };
}