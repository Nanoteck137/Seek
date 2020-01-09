#include "SeekPCH.h"
#include "Platform/Windows/WindowsFileSystem.h"

namespace Seek
{
    // NOTE(patrik): Not used, but required for ReadFileEx
    void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode,
                                          DWORD dwNumberOfBytesTransfered,
                                          LPOVERLAPPED lpOverlapped)
    {
    }

    WindowsFileSystem::WindowsFileSystem() {}

    WindowsFileSystem::~WindowsFileSystem() {}

    static HANDLE OpenFileForReading(const String& path)
    {
        HANDLE fileHandle =
            CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle == INVALID_HANDLE_VALUE)
        {
            if (GetLastError() == ERROR_FILE_NOT_FOUND)
            {
                SK_CORE_ASSERT(false, "Could not find file");
            }
            else
            {
                SK_CORE_ASSERT(false, "Unknown error");
            }
        }

        return fileHandle;
    }

    static uint64 GetFileSize(HANDLE file)
    {
        LARGE_INTEGER fileSize;
        if (GetFileSizeEx(file, &fileSize) == FALSE)
        {
            SK_CORE_ASSERT(false, "Could not get file size");
        }

        return (uint64)fileSize.QuadPart;
    }

    Buffer WindowsFileSystem::ReadAllBufferImpl(const String& path)
    {
        HANDLE fileHandle = OpenFileForReading(path);

        Buffer result = {};
        result.data = nullptr;
        result.size = 0;

        uint64 fileSize = GetFileSize(fileHandle);

        // TODO(patrik): Temp
        result.data = malloc(fileSize);
        result.size = fileSize;

        OVERLAPPED ol = {0};
        if (ReadFileEx(fileHandle, result.data, result.size, &ol,
                       FileIOCompletionRoutine) == FALSE)
        {
            SK_CORE_ASSERT(false, "Could not read file");
        }

        CloseHandle(fileHandle);

        return result;
    }

    String WindowsFileSystem::ReadAllTextImpl(const String& path)
    {
        HANDLE fileHandle = OpenFileForReading(path);

        uint64 fileSize = GetFileSize(fileHandle);
        String result(fileSize, 0);

        OVERLAPPED ol = {0};
        if (ReadFileEx(fileHandle, &result[0], fileSize, &ol,
                       FileIOCompletionRoutine) == FALSE)
        {
            SK_CORE_ASSERT(false, "Could not read file");
        }

        CloseHandle(fileHandle);

        return result;
    }

    std::vector<String> WindowsFileSystem::ReadAllLinesImpl(const String& path)
    {
        // TODO(patrik): Implement
        return std::vector<String>();
    }

    static HANDLE OpenFileForWriting(const String& path)
    {
        HANDLE fileHandle =
            CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle == INVALID_HANDLE_VALUE)
        {
            if (GetLastError() == ERROR_FILE_NOT_FOUND)
            {
                fileHandle =
                    CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL,
                                CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

                if (fileHandle == INVALID_HANDLE_VALUE)
                {
                    SK_CORE_ASSERT(false, "Could not open file");
                }
            }
            else
            {
                SK_CORE_ASSERT(false, "Unknown error");
            }
        }

        return fileHandle;
    }

    void WindowsFileSystem::WriteAllBufferImpl(const String& path,
                                               const Buffer& buffer)
    {
        HANDLE fileHandle = OpenFileForWriting(path);

        SK_CORE_ASSERT(buffer.data, "Buffer data can't be null");

        DWORD written;
        if (WriteFile(fileHandle, buffer.data, buffer.size, &written,
                      nullptr) == FALSE)
        {
            SK_CORE_ASSERT(false, "Failed to write to file");
        }

        CloseHandle(fileHandle);
    }

    void WindowsFileSystem::WriteAllTextImpl(const String& path,
                                             const String& text)
    {
        HANDLE fileHandle = OpenFileForWriting(path);

        DWORD written;
        if (WriteFile(fileHandle, text.data(), text.size(), &written,
                      nullptr) == FALSE)
        {
            SK_CORE_ASSERT(false, "Failed to write to file");
        }

        CloseHandle(fileHandle);
    }

    void WindowsFileSystem::WriteAllLinesImpl(const String& path,
                                              const std::vector<String>& lines)
    {
        // TODO(patrik): Implement
    }

    static HANDLE OpenFileForAppending(const String& path)
    {
        HANDLE fileHandle =
            CreateFileA(path.c_str(), FILE_APPEND_DATA, 0, NULL, OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle == INVALID_HANDLE_VALUE)
        {
            if (GetLastError() == ERROR_FILE_NOT_FOUND)
            {
                fileHandle =
                    CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL,
                                CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

                if (fileHandle == INVALID_HANDLE_VALUE)
                {
                    SK_CORE_ASSERT(false, "Could not open file");
                }
            }
            else
            {
                SK_CORE_ASSERT(false, "Unknown error");
            }
        }

        return fileHandle;
    }

    void WindowsFileSystem::AppendAllBufferImpl(const String& path,
                                                const Buffer& buffer)
    {
        HANDLE fileHandle = OpenFileForAppending(path);

        SK_CORE_ASSERT(buffer.data, "Buffer data can't be null");

        DWORD written;
        if (WriteFile(fileHandle, buffer.data, buffer.size, &written,
                      nullptr) == FALSE)
        {
            SK_CORE_ASSERT(false, "Failed to write to file");
        }

        CloseHandle(fileHandle);
    }

    void WindowsFileSystem::AppendAllTextImpl(const String& path,
                                              const String& text)
    {
        HANDLE fileHandle = OpenFileForAppending(path);

        DWORD written;
        if (WriteFile(fileHandle, text.data(), text.size(), &written,
                      nullptr) == FALSE)
        {
            SK_CORE_ASSERT(false, "Failed to write to file");
        }

        CloseHandle(fileHandle);
    }

    void WindowsFileSystem::AppendAllLinesImpl(const String& path,
                                               const std::vector<String>& lines)
    {
        // TODO(patrik): Implement
    }

    bool WindowsFileSystem::FileExistsImpl(const String& path)
    {
        DWORD attribs = GetFileAttributesA(path.c_str());
        return !(attribs == INVALID_FILE_ATTRIBUTES &&
                 GetLastError() == ERROR_FILE_NOT_FOUND);
    }
    std::vector<String>
    WindowsFileSystem::GetDirectoryFilesImpl(const String& path)
    {
        WIN32_FIND_DATAA findData;
        HANDLE findHandle = FindFirstFileA(path.c_str(), &findData);
        if (findHandle == INVALID_HANDLE_VALUE)
        {
            SK_CORE_ASSERT(false, "Invalid Directory");
        }

        do
        {
            SK_CORE_TRACE("{0}", String(findData.cFileName));
        } while (FindNextFileA(findHandle, &findData));

        FindClose(findHandle);

        return std::vector<String>();
    }
    std::vector<String>
    WindowsFileSystem::GetDirectorySubDirectoriesImpl(const String& path)
    {
        return std::vector<String>();
    }
}