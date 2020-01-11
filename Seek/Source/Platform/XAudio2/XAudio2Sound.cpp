#include "SeekPCH.h"
#include "Platform/XAudio2/XAudio2Sound.h"

#include "Seek/System/FileSystem.h"

namespace Seek
{

    XAudio2Sound::XAudio2Sound(XAudio2AudioEngine* engine,
                               const String& filePath)
    {
        ReadSamples(filePath);
        SetupXAudioBuffer(engine);
    }

    XAudio2Sound::~XAudio2Sound()
    {
        m_SourceVoice = nullptr;

        if (m_Samples)
        {
            delete[] m_Samples;
            m_Samples = nullptr;
        }
    }

    void XAudio2Sound::Play() { m_SourceVoice->Start(0); }

    void XAudio2Sound::Stop() { m_SourceVoice->Stop(); }

    void XAudio2Sound::ReadSamples(const String& filePath)
    {
        Buffer buffer = FileSystem::ReadAllBuffer("Assets/Sounds/test.wav");

        byte* data = (byte*)buffer.Data;
        m_Header = *(WaveHeader*)data;
        data += sizeof(WaveHeader);

        WaveChunk* chunk = (WaveChunk*)data;
        data += sizeof(WaveChunk);

        int32 sampleSize = m_Header.BitsPerSample / 8;
        SK_CORE_ASSERT(sampleSize == 2,
                       "Only supports a Sample size of 2 for now");
        m_SampleCount = (chunk->Size * 8) / m_Header.BitsPerSample;

        m_Samples = new uint16[m_SampleCount];
        memset(m_Samples, 0, m_SampleCount * sizeof(uint16));

        memcpy(m_Samples, data, m_SampleCount * sizeof(uint16));

        buffer.Free();
    }

    void XAudio2Sound::SetupXAudioBuffer(XAudio2AudioEngine* engine)
    {
        HRESULT result;

        XAUDIO2_BUFFER audioBuffer = {};
        audioBuffer.AudioBytes = m_SampleCount * sizeof(uint16);
        audioBuffer.pAudioData = (byte*)m_Samples;
        audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

        WAVEFORMATEX format;
        format.wFormatTag = m_Header.FormatType;
        format.nChannels = m_Header.Channels;
        format.nSamplesPerSec = m_Header.SampleRate;
        format.nAvgBytesPerSec = m_Header.Byterate;
        format.nBlockAlign = m_Header.BlockAlign;
        format.wBitsPerSample = m_Header.BitsPerSample;
        format.cbSize = 0;

        result =
            engine->GetDevice()->CreateSourceVoice(&m_SourceVoice, &format);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not create a source voice");

        result = m_SourceVoice->SubmitSourceBuffer(&audioBuffer);
        SK_CORE_ASSERT(SUCCEEDED(result), "Could not submit audio buffer");
    }
}