#include "SeekPCH.h"
#include "Platform/Windows/WindowsFileSystem.h"

#include "Seek/Debug/Instrumentor.h"

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
        SK_PROFILE_FUNCTION();

        LARGE_INTEGER fileSize;
        if (GetFileSizeEx(file, &fileSize) == FALSE)
        {
            SK_CORE_ASSERT(false, "Could not get file size");
        }

        return (uint64)fileSize.QuadPart;
    }

    Buffer WindowsFileSystem::ReadAllBufferImpl(const String& path)
    {
        SK_PROFILE_FUNCTION();

        HANDLE fileHandle = OpenFileForReading(path);

        Buffer result = {};
        uint64 fileSize = GetFileSize(fileHandle);

        // TODO(patrik): Temp
        // result.data = malloc(fileSize);
        // result.size = fileSize;
        result.Allocate(fileSize);

        OVERLAPPED ol = {0};
        if (ReadFileEx(fileHandle, result.Data, result.Size, &ol,
                       FileIOCompletionRoutine) == FALSE)
        {
            SK_CORE_ASSERT(false, "Could not read file");
        }

        CloseHandle(fileHandle);

        return result;
    }

    String WindowsFileSystem::ReadAllTextImpl(const String& path)
    {
        SK_PROFILE_FUNCTION();

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
        SK_PROFILE_FUNCTION();

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
        SK_PROFILE_FUNCTION();

        HANDLE fileHandle = OpenFileForWriting(path);

        SK_CORE_ASSERT(buffer.Data, "Buffer data can't be null");

        DWORD written;
        if (WriteFile(fileHandle, buffer.Data, buffer.Size, &written,
                      nullptr) == FALSE)
        {
            SK_CORE_ASSERT(false, "Failed to write to file");
        }

        CloseHandle(fileHandle);
    }

    void WindowsFileSystem::WriteAllTextImpl(const String& path,
                                             const String& text)
    {
        SK_PROFILE_FUNCTION();

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
        SK_PROFILE_FUNCTION();

        HANDLE fileHandle = OpenFileForAppending(path);

        SK_CORE_ASSERT(buffer.Data, "Buffer data can't be null");

        DWORD written;
        if (WriteFile(fileHandle, buffer.Data, buffer.Size, &written,
                      nullptr) == FALSE)
        {
            SK_CORE_ASSERT(false, "Failed to write to file");
        }

        CloseHandle(fileHandle);
    }

    void WindowsFileSystem::AppendAllTextImpl(const String& path,
                                              const String& text)
    {
        SK_PROFILE_FUNCTION();

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
        SK_PROFILE_FUNCTION();

        DWORD attribs = GetFileAttributesA(path.c_str());
        return !(attribs == INVALID_FILE_ATTRIBUTES &&
                 GetLastError() == ERROR_FILE_NOT_FOUND);
    }
    std::vector<String>
    WindowsFileSystem::GetDirectoryFilesImpl(const String& path)
    {
        SK_PROFILE_FUNCTION();

        String newPath = path + "\\*";

        WIN32_FIND_DATAA findData;
        HANDLE findHandle = FindFirstFileA(newPath.c_str(), &findData);
        if (findHandle == INVALID_HANDLE_VALUE)
        {
            SK_CORE_ASSERT(false, "Invalid Directory");
        }

        std::vector<String> result;
        do
        {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=
                FILE_ATTRIBUTE_DIRECTORY)
            {
                result.push_back(String(findData.cFileName));
            }
        } while (FindNextFileA(findHandle, &findData));

        FindClose(findHandle);

        return result;
    }
    std::vector<String>
    WindowsFileSystem::GetDirectorySubDirectoriesImpl(const String& path)
    {
        SK_PROFILE_FUNCTION();

        String newPath = path + "\\*";

        WIN32_FIND_DATAA findData;
        HANDLE findHandle = FindFirstFileA(newPath.c_str(), &findData);
        if (findHandle == INVALID_HANDLE_VALUE)
        {
            SK_CORE_ASSERT(false, "Invalid Directory");
        }

        std::vector<String> result;
        do
        {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ==
                FILE_ATTRIBUTE_DIRECTORY)
            {
                String str = String(findData.cFileName);
                if (str == "." || str == "..")
                    continue;
                result.push_back(str);
            }
        } while (FindNextFileA(findHandle, &findData));

        FindClose(findHandle);

        return result;
    }

    const static char SEPERATOR = '\\';

    String WindowsFileSystem::FixPathForSystemImpl(const String& path)
    {
        SK_PROFILE_FUNCTION();

        String result = path;
        std::replace(result.begin(), result.end(), '/', SEPERATOR);
        return result;
    }

    static String TrimPathStringStart(const String& path)
    {
        String result = path;

        while (result[0] == SEPERATOR)
        {
            result = result.substr(1, result.size());
        }

        return result;
    }

    static String TrimPathStringEnd(const String& path)
    {
        String result = path;

        while (result[result.size() - 1] == SEPERATOR)
        {
            result = result.substr(0, result.size() - 1);
        }

        return result;
    }

    String WindowsFileSystem::PathCombineImpl(const String& path,
                                              const String& path2)
    {
        SK_PROFILE_FUNCTION();

        String newPath = TrimPathStringEnd(path);
        String newPath2 = TrimPathStringStart(path2);

        String result = newPath + SEPERATOR + newPath2;

        return result;
    }

    String WindowsFileSystem::GetCurrentWorkingDirectoryPathImpl()
    {
        SK_PROFILE_FUNCTION();

        char buffer[1024] = {0};

        DWORD res = GetCurrentDirectoryA(1024, buffer);
        SK_CORE_ASSERT(res, "Could not get current directory path");

        return String(buffer);
    }

    String WindowsFileSystem::GetFullPathImpl(const String& path)
    {
        SK_PROFILE_FUNCTION();

        char buffer[1024] = {0};
        DWORD res = GetFullPathNameA(path.c_str(), 1024, buffer, nullptr);
        SK_CORE_ASSERT(res, "Could not get the full path");

        return String(buffer);
    }

    String WindowsFileSystem::GetPathFileExtensionImpl(const String& path)
    {
        SK_PROFILE_FUNCTION();

        auto pos = path.find_last_of('.');
        if (pos == String::npos)
            return String();

        String result = path.substr(pos + 1, path.size());

        return result;
    }

    String WindowsFileSystem::GetPathFileNameImpl(const String& path)
    {
        SK_PROFILE_FUNCTION();

        auto pos = path.find_last_of("/\\");
        String result = path.substr(pos + 1, path.size());

        return result;
    }

    String WindowsFileSystem::GetPathDirectoryImpl(const String& path)
    {
        SK_PROFILE_FUNCTION();

        auto pos = path.find_last_of("/\\");
        String result = path.substr(0, pos);

        return result;
    }
}