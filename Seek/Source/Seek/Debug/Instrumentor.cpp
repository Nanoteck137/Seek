#include "SeekPCH.h"
#include "Seek/Debug/Instrumentor.h"

namespace Seek
{
    Instrumentor* Instrumentor::s_Instance = new Instrumentor();

    Instrumentor::Instrumentor() {}

    Instrumentor::~Instrumentor() {}

    void Instrumentor::BeginSession(const String& name, const String& filePath)
    {
        m_OutputStream.open(filePath);
        WriteHeader();
        m_CurrentSession = new InstrumentationSession{name};
    }

    void Instrumentor::EndSession()
    {
        WriteFooter();
        m_OutputStream.close();

        delete m_CurrentSession;

        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void Instrumentor::WriteProfile(const ProfileResult& profile)
    {
        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = profile.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (profile.End - profile.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << profile.ThreadID << ",";
        m_OutputStream << "\"ts\":" << profile.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void Instrumentor::WriteHeader()
    {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void Instrumentor::WriteFooter()
    {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

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