#include "SeekPCH.h"
#include "Seek/Audio/AudioEngine.h"

#include "Platform/XAudio2/XAudio2AudioEngine.h"

namespace Seek
{
    AudioEngine* AudioEngine::s_AudioEngine = nullptr;

    void AudioEngine::Init() { s_AudioEngine = new XAudio2AudioEngine(); }

    void AudioEngine::Shutdown() { delete s_AudioEngine; }
}