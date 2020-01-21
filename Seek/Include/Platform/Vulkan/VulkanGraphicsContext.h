#pragma once

#include "Seek/Renderer/GraphicsContext.h"

namespace Seek
{
    class VulkanGraphicsContext : public
    {
    public:
        VulkanGraphicsContext();

        virtual void Init() override;
        virtual void SwapBuffers() override;
    }
}