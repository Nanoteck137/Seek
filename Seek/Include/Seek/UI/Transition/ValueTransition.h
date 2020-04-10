#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "ValueDriver.h"

namespace Seek
{
    class ValueTransition
    {
    public:
        virtual float GetHiddenValue() = 0;
        virtual Ref<ValueDriver> InitDriver(float baseValue, float currentValue,
                                            bool reverse, float delay,
                                            float totalDuration) = 0;
        virtual float GetDuration() = 0;
    };
}