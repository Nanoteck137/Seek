#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    class ValueDriver
    {
    public:
        ValueDriver(float length);
        ValueDriver(float startTime, float length);

        float Update(float deltaTime);

    protected:
        virtual float CalculateValue(float time) = 0;

    public:
        inline bool HasCompletedOnePeriod() const { return m_FirstPeriodDone; }
        inline float GetCurrentValue() const { return m_CurrentValue; }

    private:
        float m_StartTime = 0.0f;
        float m_Length = 0.0f;

        float m_CurrentTime = 0.0f;

        float m_CurrentValue = 0.0f;
        bool m_FirstPeriodDone = false;
    };
}