#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    class Timestep
    {
    public:
        Timestep(float32 time = 0.0f) : m_Time(time) {}

        operator float32() { return GetSeconds(); }

        inline float32 GetSeconds() const { return m_Time; }
        inline float32 GetMilliseconds() const { return m_Time * 1000.0f; }

    private:
        float32 m_Time;
    };
}