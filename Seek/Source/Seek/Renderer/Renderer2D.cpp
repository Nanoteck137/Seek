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

        Renderer2D::RenderStats Stats;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
        SK_CORE_ASSERT(!s_Data, "Renderer2D already initialized");
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
        SK_CORE_ASSERT(s_Data, "Renderer2D already shutdown");

        if (s_Data->VertexBufferData)
        {
            delete[] s_Data->VertexBufferData;
            s_Data->VertexBufferData = nullptr;
        }

        if (s_Data)
        {
            delete s_Data;
            s_Data = nullptr;
        }
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
        SK_CORE_ASSERT(texture, "Texture can't be null");

        float32 result = 0.0f;
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

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                              const Ref<SubTexture2D>& texture)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                              const Ref<SubTexture2D>& texture)
    {
        float textureID = SubmitTexture(texture->GetTexture());
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        auto coords = texture->GetCoords();

        // TODO/FIXME(patrik): The position is wrong for all the vertices
        // It should be (0, 0) (0, 1) (1, 1) (1, 0)
        // But it is (0, 0) (1, 0) (1, 1) (0, 1)

        s_Data->VertexBufferDataWritePtr->Position = position;
        s_Data->VertexBufferDataWritePtr->TexCoord = coords[0];
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->VertexBufferDataWritePtr->Position =
            position + glm::vec3(size.x, 0.0f, 0.0f);
        s_Data->VertexBufferDataWritePtr->TexCoord = coords[1];
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->VertexBufferDataWritePtr->Position =
            position + glm::vec3(size.x, size.y, 0.0f);
        s_Data->VertexBufferDataWritePtr->TexCoord = coords[2];
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->VertexBufferDataWritePtr->Position =
            position + glm::vec3(0.0f, size.y, 0.0f);
        s_Data->VertexBufferDataWritePtr->TexCoord = coords[3];
        s_Data->VertexBufferDataWritePtr->Color = color;
        s_Data->VertexBufferDataWritePtr->TextureID = textureID;
        s_Data->VertexBufferDataWritePtr++;

        s_Data->IndexCounter += 6;
    }

    void Renderer2D::DrawText(const glm::vec2& position, const String& text,
                              const Ref<Font>& font, const glm::vec4& color,
                              float scale)
    {
        SK_CORE_ASSERT(font, "No font");

        float textureID = SubmitTexture(font->GetTexture());

        float x = position.x;
        for (int i = 0; i < text.size(); i++)
        {
            char c = text[i];
            FontGlyph glyph = font->GetGlyphInfo(c);

            float width = glyph.Size.x;
            float height = glyph.Size.y;

            float xOffset = glyph.Offset.x;
            float yOffset = glyph.Offset.y;

            float x0 = x + xOffset * scale;
            float y0 = position.y - yOffset * scale;
            float x1 = x0 + width * scale;
            float y1 = y0 + height * scale;

            float u0 = glyph.UVRect.x;
            float v0 = glyph.UVRect.y;
            float u1 = glyph.UVRect.z;
            float v1 = glyph.UVRect.w;

            s_Data->VertexBufferDataWritePtr->Position =
                glm::vec3(x0, y0, 0.0f);
            s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(u0, v1);
            s_Data->VertexBufferDataWritePtr->Color = color;
            s_Data->VertexBufferDataWritePtr->TextureID = textureID;
            s_Data->VertexBufferDataWritePtr++;

            s_Data->VertexBufferDataWritePtr->Position =
                glm::vec3(x0, y1, 0.0f);
            s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(u0, v0);
            s_Data->VertexBufferDataWritePtr->Color = color;
            s_Data->VertexBufferDataWritePtr->TextureID = textureID;
            s_Data->VertexBufferDataWritePtr++;

            s_Data->VertexBufferDataWritePtr->Position =
                glm::vec3(x1, y1, 0.0f);
            s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(u1, v0);
            s_Data->VertexBufferDataWritePtr->Color = color;
            s_Data->VertexBufferDataWritePtr->TextureID = textureID;
            s_Data->VertexBufferDataWritePtr++;

            s_Data->VertexBufferDataWritePtr->Position =
                glm::vec3(x1, y0, 0.0f);
            s_Data->VertexBufferDataWritePtr->TexCoord = glm::vec2(u1, v1);
            s_Data->VertexBufferDataWritePtr->Color = color;
            s_Data->VertexBufferDataWritePtr->TextureID = textureID;
            s_Data->VertexBufferDataWritePtr++;

            x += glyph.XAdvance * scale;

            s_Data->IndexCounter += 6;
        }
    }

    const Renderer2D::RenderStats& Renderer2D::GetRenderStats()
    {
        return s_Data->Stats;
    }
}