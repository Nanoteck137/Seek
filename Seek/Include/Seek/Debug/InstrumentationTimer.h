#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

// TODO(patrik): Remove, use custom timer later
#include <chrono>

namespace Seek
{
    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name);
        ~InstrumentationTimer();

        void Stop();

    private:
        const char* m_Name = nullptr;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
        bool m_Stopped = false;
    };
}
