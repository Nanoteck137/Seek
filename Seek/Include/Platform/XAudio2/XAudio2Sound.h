#pragma once

#include "Seek/Audio/Sound.h"

#include "Platform/XAudio2/XAudio2AudioEngine.h"

namespace Seek
{
    struct WaveChunk
    {
        byte ID[4];
        uint32 Size;
    };

    struct WaveHeader
    {
        WaveChunk Chunk;
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

    class XAudio2Sound : public Sound
    {
    public:
        XAudio2Sound(XAudio2AudioEngine* engine, const String& filePath);
        ~XAudio2Sound();

        virtual void Play() override;
        virtual void Stop() override;

    private:
        void ReadSamples(const String& filePath);
        void SetupXAudioBuffer(XAudio2AudioEngine* engine);

    private:
        IXAudio2SourceVoice* m_SourceVoice = nullptr;

        WaveHeader m_Header;
        uint16* m_Samples = nullptr;
        uint32 m_SampleCount = 0;
    };
}