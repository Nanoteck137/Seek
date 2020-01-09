#pragma once

#include "Seek/System/FileSystem.h"

namespace Seek
{
    class WindowsFileSystem : public FileSystem
    {
    public:
        WindowsFileSystem();
        ~WindowsFileSystem();

    private:
        virtual Buffer ReadAllBufferImpl(const String& path) override;
        virtual String ReadAllTextImpl(const String& path) override;
        virtual std::vector<String>
        ReadAllLinesImpl(const String& path) override;

        virtual void WriteAllBufferImpl(const String& path,
                                        const Buffer& buffer) override;
        virtual void WriteAllTextImpl(const String& path,
                                      const String& text) override;
        virtual void
        WriteAllLinesImpl(const String& path,
                          const std::vector<String>& lines) override;

        virtual void AppendAllBufferImpl(const String& path,
                                         const Buffer& buffer) override;
        virtual void AppendAllTextImpl(const String& path,
                                       const String& text) override;
        virtual void
        AppendAllLinesImpl(const String& path,
                           const std::vector<String>& lines) override;

        virtual bool FileExistsImpl(const String& path) override;

        virtual std::vector<String>
        GetDirectoryFilesImpl(const String& path) override;
        virtual std::vector<String>
        GetDirectorySubDirectoriesImpl(const String& path) override;
    };
}