#include "SeekPCH.h"
#include "Platform/Windows/WindowsFileSystem.h"

namespace Seek
{
    WindowsFileSystem::WindowsFileSystem() {}

    WindowsFileSystem::~WindowsFileSystem() {}

    Buffer WindowsFileSystem::ReadAllBufferImpl(const String& path)
    {
        return Buffer();
    }

    String WindowsFileSystem::ReadAllTextImpl(const String& path)
    {
        return String();
    }

    std::vector<String> WindowsFileSystem::ReadAllLinesImpl(const String& path)
    {
        return std::vector<String>();
    }

    void WindowsFileSystem::WriteAllBufferImpl(const String& path,
                                               const Buffer& buffer)
    {
    }

    void WindowsFileSystem::WriteAllTextImpl(const String& path,
                                             const String& text)
    {
    }

    void WindowsFileSystem::WriteAllLinesImpl(const String& path,
                                              const std::vector<String>& lines)
    {
    }
}