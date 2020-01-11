#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    class Sound;

    class AudioEngine
    {
    public:
        virtual ~AudioEngine() = default;

        virtual Ref<Sound> CreateSound(const String& filePath) = 0;

    public:
        static void Init();
        static void Shutdown();

    private:
        static AudioEngine* s_AudioEngine;
    };
}