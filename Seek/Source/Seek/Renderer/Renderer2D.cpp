#include "SeekPCH.h"
#include "Seek/Renderer/Renderer2D.h"

#include "Seek/Renderer/VertexArray.h"
#include "Seek/Renderer/Shader.h"
#include "Seek/Renderer/Texture.h"

#include "Seek/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Seek
{
    struct Vertex2D
    {
        glm::vec3 Position;
        glm::vec2 TexCoord;
        glm::vec4 Color;
        float32 TextureID;
    };

    struct Renderer2DStorage
    {
        Ref<VertexArray> VertexArray;
        Ref<VertexBuffer> VertexBuffer;
        Ref<IndexBuffer> IndexBuffer;

        Ref<Texture2D> WhiteTexture;
        std::vector<Ref<Texture2D>> Textures;

        Vertex2D* VertexBufferData = nullptr;
        Vertex2D* VertexBufferDataWritePtr = nullptr;

        Ref<Shader> Shader;

        uint32 IndexCounter = 0;

        RenderStats Stats;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
        s_Data = new Renderer2DStorage();

        const uint32 MAX_QUADS = 100000;
        const uint32 VERTEX_BUFFER_SIZE = MAX_QUADS * sizeof(Vertex2D) * 4;
        const uint32 INDEX_BUFFER_SIZE = MAX_QUADS * 6;

        s_Data->VertexArray = VertexArray::Create();

        s_Data->VertexBuffer = VertexBuffer::Create(BufferUsage::Dynamic);
        s_Data->VertexBuffer->Resize(VERTEX_BUFFER_SIZE);

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float, "a_TextureID"},
        };

        s_Data->VertexBuffer->SetLayout(layout);

        s_Data->VertexArray->AddVertexBuffer(s_Data->VertexBuffer);

        s_Data->VertexBufferData = new Vertex2D[MAX_QUADS * 4];
        s_Data->VertexBufferDataWritePtr = s_Data->VertexBufferData;

        uint32* indicies = new uint32[INDEX_BUFFER_SIZE];

        uint32 offset = 0;
        for (uint32 i = 0; i < INDEX_BUFFER_SIZE; i += 6)
        {
            indicies[i + 0] = offset + 0;
            indicies[i + 1] = offset + 1;
            indicies[i + 2] = offset + 2;

            indicies[i + 3] = offset + 2;
            indicies[i + 4] = offset + 3;
            indicies[i + 5] = offset + 0;

            offset += 4;
        }

        s_Data->IndexBuffer = IndexBuffer::Create(indicies, INDEX_BUFFER_SIZE);
        s_Data->VertexArray->AddIndexBuffer(s_Data->IndexBuffer);

        s_Data->Shader = Shader::Create("Assets/Shaders/Renderer2D.glsl");
        s_Data->Shader->Bind();
        int32 values[32] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                            11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                            22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
        s_Data->Shader->SetUnifromIntArray("u_Textures", values, 32);

        TextureParameters params = {};
        s_Data->WhiteTexture = Texture2D::Create(1, 1, params);

        uint8 data[4] = {0xff, 0xff, 0xff, 0xff};
        s_Data->WhiteTexture->SetData(data, sizeof(data));

        ResetRenderStats();
    }

    void Renderer2D::Shutdown()
    {
        delete[] s_Data->VertexBufferData;
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_Data->VertexBufferDataWritePtr = s_Data->VertexBufferData;
        s_Data->IndexCounter = 0;
        s_Data->Textures.clear();

        s_Data->Shader->Bind();
        s_Data->Shader->SetUniformMatrix4("u_ViewProjection",
                                          camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
        size_t vertexBufferDataSize = (byte*)s_Data->VertexBufferDataWritePtr -
                                      (byte*)s_Data->VertexBufferData;
        if (vertexBufferDataSize != 0)
        {
            s_Data->VertexBuffer->SetData(s_Data->VertexBufferData,
                                          vertexBufferDataSize);
        }
    }

    void Renderer2D::Flush()
    {
        for (int i = 0; i < s_Data->Textures.size(); i++)
        {
            s_Data->Textures[i]->Bind(i);
        }

        RenderCommand::DrawIndexed(s_Data->VertexArray, s_Data->IndexCounter);
    }

    void Renderer2D::ResetRenderStats()
    {
        memset(&s_Data->Stats, 0, sizeof(RenderStats));
    }

    float Renderer2D::SubmitTexture(const Ref<Texture2D>& texture)
    {
        float result = 0.0f;
        bool found = false;
        for (uint32 i = 0; i < s_Data->Textures.size(); i++)
        {
            if (s_Data->Textures[i] == texture)
            {
                result = (float)(i + 1);
                found = true;
                break;
            }
        }

        if (!found)
        {
            if (s_Data->Textures.size() >= 32)
            {
                SK_CORE_ASSERT(false, "Too many textures");
            }

            s_Data->Textures.push_back(texture);
            result = (float)(s_Data->Textures.size());
        }

        return result;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                              const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                              const glm::vec4& color)
    {
        float textureID = SubmitTexture(s_Data->WhiteTexture);

        s_Data->VertexBufferDataWritePtr->Position = position;
        s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(0.0f, 0.0f);
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->VertexBufferDataWritePtr->Position =
            position + glm::vec3(size.x, 0.0f, 0.0f);
        s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(1.0f, 0.0f);
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->VertexBufferDataWritePtr->Position =
            position + glm::vec3(size.x, size.y, 0.0f);
        s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(1.0f, 1.0f);
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->VertexBufferDataWritePtr->Position =
            position + glm::vec3(0.0f, size.y, 0.0f);
        s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(0.0f, 1.0f);
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->IndexCounter += 6;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                              const Ref<Texture2D>& texture)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                              const Ref<Texture2D>& texture)
    {
        float textureID = SubmitTexture(texture);
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        s_Data->VertexBufferDataWritePtr->Position = position;
        s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(0.0f, 0.0f);
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->VertexBufferDataWritePtr->Position =
            position + glm::vec3(size.x, 0.0f, 0.0f);
        s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(1.0f, 0.0f);
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->VertexBufferDataWritePtr->Position =
            position + glm::vec3(size.x, size.y, 0.0f);
        s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(1.0f, 1.0f);
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->VertexBufferDataWritePtr->Position =
            position + glm::vec3(0.0f, size.y, 0.0f);
        s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(0.0f, 1.0f);
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->IndexCounter += 6;
    }

    void Renderer2D::DrawText(const glm::vec2& position, const String& text,
                              const Ref<Font>& font)
    {
        float textureID = SubmitTexture(font->GetTexture());

        float x = position.x;
        float y = position.y;
        for (int i = 0; i < text.size(); i++)
        {
            char c = text[i];
            FontGlyph glyph = font->GetGlyphInfo(c, x, y);

            x = glyph.Offset.x;
            y = glyph.Offset.y;

            glm::vec4 color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

            s_Data->VertexBufferDataWritePtr->Position =
                glm::vec3(glyph.Pos0.x, glyph.Pos0.y, 0.0f);
            s_Data->VertexBufferDataWritePtr->TexCoord =
                glm::vec2(glyph.TexCoord0.x, glyph.TexCoord1.y);
            s_Data->VertexBufferDataWritePtr->Color = color;
            s_Data->VertexBufferDataWritePtr->TextureID = textureID;
            s_Data->VertexBufferDataWritePtr++;

            s_Data->VertexBufferDataWritePtr->Position =
                glm::vec3(glyph.Pos0.x, glyph.Pos1.y, 0.0f);
            s_Data->VertexBufferDataWritePtr->TexCoord =
                glm::vec2(glyph.TexCoord0.x, glyph.TexCoord0.y);
            s_Data->VertexBufferDataWritePtr->Color = color;
            s_Data->VertexBufferDataWritePtr->TextureID = textureID;
            s_Data->VertexBufferDataWritePtr++;

            s_Data->VertexBufferDataWritePtr->Position =
                glm::vec3(glyph.Pos1.x, glyph.Pos1.y, 0.0f);
            s_Data->VertexBufferDataWritePtr->TexCoord =
                glm::vec2(glyph.TexCoord1.x, glyph.TexCoord0.y);
            s_Data->VertexBufferDataWritePtr->Color = color;
            s_Data->VertexBufferDataWritePtr->TextureID = textureID;
            s_Data->VertexBufferDataWritePtr++;

            s_Data->VertexBufferDataWritePtr->Position =
                glm::vec3(glyph.Pos1.x, glyph.Pos0.y, 0.0f);
            s_Data->VertexBufferDataWritePtr->TexCoord =
                glm::vec2(glyph.TexCoord1.x, glyph.TexCoord1.y);
            s_Data->VertexBufferDataWritePtr->Color = color;
            s_Data->VertexBufferDataWritePtr->TextureID = textureID;
            s_Data->VertexBufferDataWritePtr++;

            s_Data->IndexCounter += 6;
        }
    }

    const RenderStats& Renderer2D::GetRenderStats() { return s_Data->Stats; }
}