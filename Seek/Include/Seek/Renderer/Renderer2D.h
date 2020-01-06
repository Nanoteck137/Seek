#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/OrthographicCamera.h"
#include "Seek/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Seek
{
    struct RenderStats
    {
        uint32 NumQuads;
        uint32 NumDrawCalls;
    };

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();

        static void ResetRenderStats();

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                             const glm::vec4& color);

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                             const Ref<Texture2D>& texture);

        static const RenderStats& GetRenderStats();

    private:
        static float SubmitTexture(const Ref<Texture2D>& texture);
    };
}