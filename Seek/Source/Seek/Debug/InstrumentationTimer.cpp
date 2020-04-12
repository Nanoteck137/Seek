#include "SeekPCH.h"
#include "Seek/Debug/InstrumentationTimer.h"

#include "Seek/Debug/Instrumentor.h"

namespace Seek
{
    InstrumentationTimer::InstrumentationTimer(const char* name) : m_Name(name)
    {
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

    InstrumentationTimer::~InstrumentationTimer()
    {
        if (!m_Stopped)
            Stop();
    }

    void InstrumentationTimer::Stop()
    {
        auto endTime = std::chrono::high_resolution_clock::now();

        int64 start =
            std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime)
                .time_since_epoch()
                .count();

        int64 end =
            std::chrono::time_point_cast<std::chrono::microseconds>(endTime)
                .time_since_epoch()
                .count();

        // TODO(patrik): Include ThreadID
        Instrumentor::Get().WriteProfile({m_Name, start, end, 0});

        m_Stopped = true;
    }
}