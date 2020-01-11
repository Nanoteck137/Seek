#include "SeekPCH.h"
#include "Platform/XAudio2/XAudio2AudioEngine.h"

#include "Seek/System/FileSystem.h"

namespace Seek
{
    struct WaveHeader
    {
        byte RIFF[4];
        uint32 OverallSize;
        byte Wave[4];
        byte FormatChunkMarker[4];
        uint32 LengthOfFormat;
        uint16 FormatType;
        uint16 Channels;
        uint32 SampleRate;
        uint32 Byterate;
        uint16 BlockAlign;
        uint16 BitsPerSample;
    };

    struct WaveChunk
    {
        byte ID[4];
        uint32 Size;
    };

    XAudio2AudioEngine::XAudio2AudioEngine()
    {
        HRESULT result;
        result = CoInitializeEx(0, COINIT_MULTITHREADED);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not CoInitialize");

        result = XAudio2Create(&m_XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not initialize XAudio2");

        result = m_XAudio2->CreateMasteringVoice(&m_MasterVoice);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not create a master voice");

        Buffer buffer = FileSystem::ReadAllBuffer("Assets/Sounds/test.wav");

        byte* data = (byte*)buffer.data;
        WaveHeader* header = (WaveHeader*)data;
        data += sizeof(WaveHeader);

        WaveChunk* chunk = (WaveChunk*)data;
        data += sizeof(WaveChunk);

        int32 sampleSize = header->BitsPerSample / 8;
        int32 sampleCount = (chunk->Size * 8) / header->BitsPerSample;

        uint16* samples = new uint16[sampleCount];
        memset(samples, 0, sampleCount * sizeof(uint16));

        memcpy(samples, data, sampleCount * sizeof(uint16));

        XAUDIO2_BUFFER audioBuffer = {};
        audioBuffer.AudioBytes = sampleCount * sizeof(uint16);
        audioBuffer.pAudioData = (byte*)samples;
        audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

        WAVEFORMATEX format;
        format.wFormatTag = header->FormatType;
        format.nChannels = header->Channels;
        format.nSamplesPerSec = header->SampleRate;
        format.nAvgBytesPerSec = header->Byterate;
        format.nBlockAlign = header->BlockAlign;
        format.wBitsPerSample = header->BitsPerSample;
        format.cbSize = 0;

        IXAudio2SourceVoice* sourceVoice = nullptr;
        result = m_XAudio2->CreateSourceVoice(&sourceVoice, &format);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not create a source voice");

        result = sourceVoice->SubmitSourceBuffer(&audioBuffer);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not submit audio buffer");

        sourceVoice->Start(0);
    }

    XAudio2AudioEngine::~XAudio2AudioEngine()
    {
        m_MasterVoice = nullptr;

        if (m_XAudio2)
        {
            m_XAudio2->Release();
            m_XAudio2 = nullptr;
        }
    }

    Ref<Sound> XAudio2AudioEngine::CreateSound(const String& filePath)
    {
        return Ref<Sound>();
    }
}