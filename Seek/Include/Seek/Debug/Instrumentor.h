#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <fstream>

// TODO(patrik): Remove, use custom timer later
#include <chrono>

namespace Seek
{
    struct ProfileResult
    {
        String Name;
        int64 Start;
        int64 End;
        uint32 ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    public:
        Instrumentor();
        ~Instrumentor();

        void BeginSession(const String& name,
                          const String& filePath = "result.json");
        void EndSession();
        void WriteProfile(const ProfileResult& profile);

    public:
        static Instrumentor& Get() { return *s_Instance; }

    private:
        void WriteHeader();
        void WriteFooter();

    private:
        InstrumentationSession* m_CurrentSession = nullptr;
        // TODO(patrik): Change this
        std::ofstream m_OutputStream;
        int m_ProfileCount = 0;

    private:
        static Instrumentor* s_Instance;
    };

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

#define SK_PROFILE_BEGIN_SESSION(name, filepath)                               \
    ::Seek::Instrumentor::Get().BeginSession(name, filepath)
#define SK_PROFILE_END_SESSION() ::Seek::Instrumentor::Get().EndSession()
#define SK_PROFILE_SCOPE(name)                                                 \
    ::Seek::InstrumentationTimer timer##__LINE__(name);
#define SK_PROFILE_FUNCTION() SK_PROFILE_SCOPE(__FUNCSIG__)
}