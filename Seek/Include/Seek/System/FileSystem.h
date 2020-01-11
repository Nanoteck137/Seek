#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    struct Buffer
    {
        void* Data;
        uint64 Size;

        void Allocate(uint64 size);
        void Free();
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

        static inline void AppendAllBuffer(const String& path,
                                           const Buffer& buffer)
        {
            s_Instance->AppendAllBufferImpl(path, buffer);
        }

        static inline void AppendAllText(const String& path, const String& text)
        {
            s_Instance->AppendAllText(path, text);
        }

        static inline void AppendAllLines(const String& path,
                                          const std::vector<String>& lines)
        {
            s_Instance->AppendAllLinesImpl(path, lines);
        }

        static inline bool FileExists(const String& path)
        {
            return s_Instance->FileExistsImpl(path);
        }

        static inline std::vector<String> GetDirectoryFiles(const String& path)
        {
            return s_Instance->GetDirectoryFilesImpl(path);
        }

        static inline std::vector<String>
        GetDirectorySubDirectories(const String& path)
        {
            return s_Instance->GetDirectorySubDirectoriesImpl(path);
        }

        static inline String FixPathForSystem(const String& path)
        {
            return s_Instance->FixPathForSystemImpl(path);
        }

        static inline String PathCombine(const String& path,
                                         const String& path2)
        {
            return s_Instance->PathCombineImpl(path, path2);
        }

        static inline String GetCurrentWorkingDirectoryPath()
        {
            return s_Instance->GetCurrentWorkingDirectoryPathImpl();
        }

        static inline String GetFullPath(const String& path)
        {
            return s_Instance->GetFullPathImpl(path);
        }

        static inline String GetPathFileExtension(const String& path)
        {
            return s_Instance->GetPathFileExtensionImpl(path);
        }

        static inline String GetPathFileName(const String& path)
        {
            return s_Instance->GetPathFileNameImpl(path);
        }

        static inline String GetPathDirectory(const String& path)
        {
            return s_Instance->GetPathDirectoryImpl(path);
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

        virtual void AppendAllBufferImpl(const String& path,
                                         const Buffer& buffer) = 0;
        virtual void AppendAllTextImpl(const String& path,
                                       const String& text) = 0;
        virtual void AppendAllLinesImpl(const String& path,
                                        const std::vector<String>& lines) = 0;

        virtual bool FileExistsImpl(const String& path) = 0;

        virtual std::vector<String>
        GetDirectoryFilesImpl(const String& path) = 0;
        virtual std::vector<String>
        GetDirectorySubDirectoriesImpl(const String& path) = 0;

        virtual String FixPathForSystemImpl(const String& path) = 0;
        virtual String PathCombineImpl(const String& path,
                                       const String& path2) = 0;
        virtual String GetCurrentWorkingDirectoryPathImpl() = 0;
        virtual String GetFullPathImpl(const String& path) = 0;
        virtual String GetPathFileExtensionImpl(const String& path) = 0;
        virtual String GetPathFileNameImpl(const String& path) = 0;
        virtual String GetPathDirectoryImpl(const String& path) = 0;

    private:
        static FileSystem* s_Instance;
    };
}