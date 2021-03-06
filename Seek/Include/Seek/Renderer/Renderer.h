#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/RenderCommand.h"
#include "Seek/Renderer/Shader.h"

#include "Seek/Renderer/OrthographicCamera.h"

namespace Seek
{
    class Renderer
    {
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
        };

    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const Ref<Shader>& shader,
                           const Ref<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }

    private:
        static SceneData* s_SceneData;
    };
}