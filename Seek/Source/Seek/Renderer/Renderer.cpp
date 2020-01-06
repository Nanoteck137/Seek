#include "SeekPCH.h"
#include "Seek/Renderer/Renderer.h"

#include "Seek/Renderer/Renderer2D.h"

namespace Seek
{
    Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown() {}

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {}

    void Renderer::Submit(const Ref<Shader>& shader,
                          const Ref<VertexArray>& vertexArray)
    {
        shader->Bind();
        shader->SetUniformMatrix4("u_ViewProjection",
                                  s_SceneData->ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}