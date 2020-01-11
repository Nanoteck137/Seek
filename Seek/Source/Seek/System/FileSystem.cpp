#include "SeekPCH.h"
#include "Seek/System/FileSystem.h"

#include "Platform/Windows/WindowsFileSystem.h"

namespace Seek
{
    FileSystem* FileSystem::s_Instance = new WindowsFileSystem();

    void Buffer::Allocate(uint64 size)
    {
        SK_CORE_ASSERT(!Data, "Data already allocated");

        Data = new byte[size];
        Size = size;
    }

    void Buffer::Free()
    {
        if (Data)
        {
            delete[] Data;
            Data = nullptr;
            Size = 0;
        }
    }
}