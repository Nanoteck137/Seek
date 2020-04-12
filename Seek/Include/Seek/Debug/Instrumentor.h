#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <fstream>

#include "InstrumentationTimer.h"

namespace Seek
{
    class Instrumentor
    {
    public:
        struct ProfileResult
        {
            String Name;
            int64 Start;
            int64 End;
            uint32 ThreadID;
        };

        struct Session
        {
            String Name;
        };

    public:
        Instrumentor();
        ~Instrumentor();

    public:
        void BeginSession(const String& name,
                          const String& filePath = "result.json");
        void EndSession();
        void WriteProfile(const ProfileResult& profile);

    private:
        void WriteHeader();
        void WriteFooter();

    private:
        Session* m_CurrentSession = nullptr;
        // TODO(patrik): Change this
        std::ofstream m_OutputStream;
        int32 m_ProfileCount = 0;

    public:
        static Instrumentor& Get() { return *s_Instance; }

    private:
        static Instrumentor* s_Instance;
    };

#define SK_PROFILE_BEGIN_SESSION(name, filepath)                               \
    ::Seek::Instrumentor::Get().BeginSession(name, filepath)
#define SK_PROFILE_END_SESSION() ::Seek::Instrumentor::Get().EndSession()
#define SK_PROFILE_SCOPE(name)                                                 \
    ::Seek::InstrumentationTimer timer##__LINE__(name);
#define SK_PROFILE_FUNCTION() SK_PROFILE_SCOPE(__FUNCSIG__)
}