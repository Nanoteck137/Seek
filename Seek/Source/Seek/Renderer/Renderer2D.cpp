#include "SeekPCH.h"
#include "Seek/Renderer/Renderer2D.h"

#include "Seek/Renderer/VertexArray.h"
#include "Seek/Renderer/Shader.h"

#include "Seek/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Seek
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        /*Ref<Shader> FlatColorShader;
        Ref<Shader> TextureShader;*/
        Ref<Shader> Shader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
        s_Data = new Renderer2DStorage();

        s_Data->QuadVertexArray = VertexArray::Create();

        struct Vertex
        {
            glm::vec3 pos;
            glm::vec2 texCoord;
        };

        Vertex vertices[] = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}}, //
            {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},  //
            {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},   //
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}},  //
        };

        Ref<VertexBuffer> vertexBuffer =
            VertexBuffer::Create(vertices, sizeof(vertices));

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"},
        };

        vertexBuffer->SetLayout(layout);

        s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

        uint32 indicies[] = {0, 1, 2, 2, 3, 0};

        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indicies, 6);
        s_Data->QuadVertexArray->AddIndexBuffer(indexBuffer);

        /*s_Data->FlatColorShader =
            Shader::Create("Assets/Shaders/FlatColor.glsl");
        s_Data->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");*/

        s_Data->Shader = Shader::Create("Assets/Shaders/Renderer2D.glsl");

        s_Data->WhiteTexture = Texture2D::Create(1, 1);

        uint8 data[4] = {0xff, 0xff, 0xff, 0xff};
        s_Data->WhiteTexture->SetData(data, sizeof(data));
    }

    void Renderer2D::Shutdown() { delete s_Data; }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        /*s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetUniformMatrix4(
            "u_ViewProjection", camera.GetViewProjectionMatrix());

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetUniformMatrix4(
            "u_ViewProjection", camera.GetViewProjectionMatrix());
        s_Data->TextureShader->SetUniformInt("u_Texture", 0);*/

        s_Data->Shader->Bind();
        s_Data->Shader->SetUniformMatrix4("u_ViewProjection",
                                          camera.GetViewProjectionMatrix());
        s_Data->Shader->SetUniformInt("u_Texture", 0);
    }

    void Renderer2D::EndScene() {}

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                              const glm::vec4& color)
    {

        s_Data->Shader->SetUniformFloat4("u_Color", color);

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_Data->Shader->SetUniformMatrix4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        s_Data->WhiteTexture->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                              const Ref<Texture2D>& texture)
    {
        s_Data->Shader->SetUniformFloat4("u_Color", {1.0f, 1.0f, 1.0f, 1.0f});

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_Data->Shader->SetUniformMatrix4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        texture->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}