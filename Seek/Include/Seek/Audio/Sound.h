#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    class Sound
    {
    public:
        virtual ~Sound() = default;

        virtual void Play() = 0;
        virtual void Stop() = 0;
    };
}