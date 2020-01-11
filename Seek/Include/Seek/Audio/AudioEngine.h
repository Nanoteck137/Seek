#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    class Sound;

    class AudioEngine
    {
    public:
        static void Init();
        static void Shutdown();

        static Ref<Sound> CreateSound(const String& filePath)
        {
            return s_Instance->CreateSoundImpl(filePath);
        }

    public:
        virtual ~AudioEngine() = default;

        virtual Ref<Sound> CreateSoundImpl(const String& filePath) = 0;

    private:
        static AudioEngine* s_Instance;
    };
}