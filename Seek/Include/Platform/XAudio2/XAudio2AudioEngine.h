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

        virtual Ref<Sound> CreateSound(const String& filePath) override;

    private:
        IXAudio2* m_XAudio2 = nullptr;
        IXAudio2MasteringVoice* m_MasterVoice = nullptr;
    };
}