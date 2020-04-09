#include "SeekPCH.h"
#include "Seek/UI/Transition/SlideDriver.h"

namespace Seek
{
    SlideDriver::SlideDriver(float start, float end, float length)
        : ValueDriver(length), m_StartValue(start), m_EndValue(length)
    {
    }

    SlideDriver::SlideDriver(float start, float end, float length,
                             float timeDelay)
        : ValueDriver(timeDelay, length), m_StartValue(start), m_EndValue(end)
    {
    }

    const double PI = 3.14159265359;

    float CosInterpolate(float a, float b, float blend)
    {
        double ft = blend * PI;
        float f = (float)((1.0f - cos(ft)) * 0.5f);
        return a * (1 - f) + b * f;
    }

    float SlideDriver::CalculateValue(float time)
    {
        if (HasCompletedOnePeriod())
            return m_EndValue;

        return CosInterpolate(m_StartValue, m_EndValue, time);
    }
}