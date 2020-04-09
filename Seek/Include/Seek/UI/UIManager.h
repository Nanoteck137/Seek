#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/UI/Components/UIContainer.h"

namespace Seek
{
    class UIManager
    {
    public:
        static void Init();
        static void Shutdown();

        static void UpdateDisplaySize(int32 width, int32 height);

        static int32 GetDisplayWidth();
        static int32 GetDisplayHeight();

        static UIContainer* GetContainer();
    };
}