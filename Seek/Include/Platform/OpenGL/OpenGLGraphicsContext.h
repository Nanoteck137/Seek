#pragma once

#include "Seek/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Seek
{
    class OpenGLGraphicsContext : public GraphicsContext
    {
    public:
        OpenGLGraphicsContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void Shutdown() override;

        virtual void Present() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}