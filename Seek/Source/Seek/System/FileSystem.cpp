#include "SeekPCH.h"
#include "Seek/System/FileSystem.h"

#include "Platform/Windows/WindowsFileSystem.h"

namespace Seek
{
    FileSystem* FileSystem::s_Instance = new WindowsFileSystem();
}