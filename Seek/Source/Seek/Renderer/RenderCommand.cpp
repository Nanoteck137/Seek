#include "SeekPCH.h"
#include "Seek/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Seek
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}