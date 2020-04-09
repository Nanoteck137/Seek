#include "SeekPCH.h"
#include "Seek/UI/Transition/ValueDriver.h"

namespace Seek
{
    ValueDriver::ValueDriver(float length) : m_Length(length), m_StartTime(0) {}

    ValueDriver::ValueDriver(float startTime, float length)
        : m_StartTime(startTime), m_Length(length)
    {
    }

    float ValueDriver::Update(float deltaTime)
    {
        m_CurrentTime += deltaTime;

        if (m_CurrentTime < m_StartTime)
        {
            m_CurrentValue = CalculateValue(0);
            return m_CurrentValue;
        }

        float totalTime = m_Length + m_StartTime;

        if (m_CurrentTime >= totalTime)
        {
            m_CurrentTime = fmod(m_CurrentTime, totalTime);
            m_FirstPeriodDone = true;
        }

        float relativeTime = (m_CurrentTime - m_StartTime) / m_Length;
        m_CurrentValue = CalculateValue(relativeTime);

        return m_CurrentValue;
    }
}