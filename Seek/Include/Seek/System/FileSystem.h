#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    struct Buffer
    {
        void* data;
        uint64 size;
    };

    class FileSystem
    {
    public:
        virtual ~FileSystem() = default;

    public:
        static inline Buffer ReadAllBuffer(const String& path)
        {
            return s_Instance->ReadAllBufferImpl(path);
        }

        static inline String ReadAllText(const String& path)
        {
            return s_Instance->ReadAllTextImpl(path);
        }

        static inline std::vector<String> ReadAllLines(const String& path)
        {
            return s_Instance->ReadAllLinesImpl(path);
        }

        static inline void WriteAllBuffer(const String& path,
                                          const Buffer& buffer)
        {
            s_Instance->WriteAllBufferImpl(path, buffer);
        }

        static inline void WriteAllText(const String& path, const String& text)
        {
            s_Instance->WriteAllTextImpl(path, text);
        }

        static inline void WriteAllLines(const String& path,
                                         const std::vector<String>& lines)
        {
            s_Instance->WriteAllLinesImpl(path, lines);
        }

    private:
        virtual Buffer ReadAllBufferImpl(const String& path) = 0;
        virtual String ReadAllTextImpl(const String& path) = 0;
        virtual std::vector<String> ReadAllLinesImpl(const String& path) = 0;

        virtual void WriteAllBufferImpl(const String& path,
                                        const Buffer& buffer) = 0;
        virtual void WriteAllTextImpl(const String& path,
                                      const String& text) = 0;
        virtual void WriteAllLinesImpl(const String& path,
                                       const std::vector<String>& lines) = 0;

    private:
        static FileSystem* s_Instance;
    };
}