#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "ValueTransition.h"

namespace Seek
{
    class SlideTransition : public ValueTransition
    {
    public:
        SlideTransition(float offset, float duration);
        SlideTransition(float offset, float duration, float selfDelay);

        virtual float GetHiddenValue() override;
        virtual ValueDriver* InitDriver(float baseValue, float currentValue,
                                        bool reverse, float delay,
                                        float totalDuration) override;
        virtual float GetDuration() override;

    private:
        float m_OffsetValue = 0.0f;
        float m_Duration = 0.0f;
        float m_SelfDelay = 0.0f;
    };
}