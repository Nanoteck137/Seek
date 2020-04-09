#include "SeekPCH.h"
#include "Seek/UI/Transition/SlideTransition.h"

#include "Seek/UI/Transition/SlideDriver.h"

namespace Seek
{
    SlideTransition::SlideTransition(float offset, float duration)
        : m_OffsetValue(offset), m_Duration(duration), m_SelfDelay(0)
    {
    }

    SlideTransition::SlideTransition(float offset, float duration,
                                     float selfDelay)
        : m_OffsetValue(offset), m_Duration(duration), m_SelfDelay(selfDelay)
    {
    }

    float SlideTransition::GetHiddenValue() { return m_OffsetValue; }

    ValueDriver* SlideTransition::InitDriver(float baseValue,
                                             float currentValue, bool reverse,
                                             float delay, float totalDuration)
    {
        float target = reverse ? baseValue : m_OffsetValue;
        float selfDelay =
            reverse ? totalDuration - (m_SelfDelay + m_Duration) : m_SelfDelay;
        return new SlideDriver(currentValue, target, m_Duration,
                               delay + selfDelay);
    }

    float SlideTransition::GetDuration() { return m_Duration + m_SelfDelay; }
}