#include "SeekPCH.h"
#include "Seek/Audio/AudioEngine.h"

#include "Platform/XAudio2/XAudio2AudioEngine.h"

namespace Seek
{
    AudioEngine* AudioEngine::s_Instance = nullptr;

    void AudioEngine::Init() { s_Instance = new XAudio2AudioEngine(); }

    void AudioEngine::Shutdown() { delete s_Instance; }
}