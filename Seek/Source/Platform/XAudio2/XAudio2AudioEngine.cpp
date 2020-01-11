#include "SeekPCH.h"
#include "Platform/XAudio2/XAudio2AudioEngine.h"

#include "Platform/XAudio2/XAudio2Sound.h"

#include "Seek/System/FileSystem.h"

namespace Seek
{

    XAudio2AudioEngine::XAudio2AudioEngine()
    {
        HRESULT result;
        result = CoInitializeEx(0, COINIT_MULTITHREADED);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not CoInitialize");

        result = XAudio2Create(&m_Device, 0, XAUDIO2_DEFAULT_PROCESSOR);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not initialize XAudio2");

        result = m_Device->CreateMasteringVoice(&m_MasterVoice);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not create a master voice");
    }

    XAudio2AudioEngine::~XAudio2AudioEngine()
    {
        m_MasterVoice = nullptr;

        if (m_Device)
        {
            m_Device->Release();
            m_Device = nullptr;
        }
    }

    Ref<Sound> XAudio2AudioEngine::CreateSoundImpl(const String& filePath)
    {
        return CreateRef<XAudio2Sound>(this, filePath);
    }
}