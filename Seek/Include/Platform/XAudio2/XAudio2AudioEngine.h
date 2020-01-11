#pragma once

#include "Seek/Audio/AudioEngine.h"

#include <xaudio2.h>

namespace Seek
{
    class XAudio2AudioEngine : public AudioEngine
    {
    public:
        XAudio2AudioEngine();
        ~XAudio2AudioEngine();

        virtual Ref<Sound> CreateSoundImpl(const String& filePath) override;

        inline IXAudio2* GetDevice() const { return m_Device; }

    private:
        IXAudio2* m_Device = nullptr;
        IXAudio2MasteringVoice* m_MasterVoice = nullptr;
    };
}