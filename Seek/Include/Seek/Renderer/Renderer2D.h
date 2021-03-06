#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/OrthographicCamera.h"
#include "Seek/Renderer/Texture.h"
#include "Seek/Renderer/SubTexture2D.h"

#include "Seek/Renderer/Font.h"

#include <glm/glm.hpp>

#undef DrawText

namespace Seek
{
    class Renderer2D
    {
    public:
        struct RenderStats
        {
            uint32 NumQuads;
            uint32 NumDrawCalls;
        };

    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();

        static void ResetRenderStats();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                             const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                             const glm::vec4& color);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                             const Ref<Texture2D>& texture);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                             const Ref<Texture2D>& texture);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                             const Ref<SubTexture2D>& texture);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                             const Ref<SubTexture2D>& texture);

        static void DrawText(const glm::vec2& position, const String& text,
                             const Ref<Font>& font, const glm::vec4& color,
                             float scale = 1.0f);

        static const RenderStats& GetRenderStats();

    private:
        static float SubmitTexture(const Ref<Texture2D>& texture);
    };
}