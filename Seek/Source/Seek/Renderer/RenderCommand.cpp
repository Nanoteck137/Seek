#include "SeekPCH.h"
#include "Seek/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Seek
{
    RendererAPI* RenderCommand::s_RendererAPI;

    void RenderCommand::Init()
    {
        s_RendererAPI = new OpenGLRendererAPI();
        s_RendererAPI->Init();
    }

    void RenderCommand::Shutdown() { s_RendererAPI->Shutdown(); }
}