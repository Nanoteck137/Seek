#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    class GraphicsContext
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}