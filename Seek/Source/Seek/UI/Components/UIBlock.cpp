#include "SeekPCH.h"
#include "Seek/UI/Components/UIBlock.h"

#include "Seek/UI/UIManager.h"

#include "Seek/Renderer/Renderer2D.h"

namespace Seek
{
    UIBlock::UIBlock() {}

    UIBlock::~UIBlock() {}

    void UIBlock::OnUpdate(Timestep ts)
    {
        float32 alpha = m_TotalAlpha;

        glm::vec2 position = GetPositionInPixels();
        glm::vec2 size = GetSizeInPixels();

        if (m_Border)
        {
            glm::vec4 borderColor = m_BorderColor;
            borderColor.a = m_Color.a;
            borderColor.a *= alpha;
            Renderer2D::DrawQuad(position, size, borderColor);

            size -= m_BorderThickness;
            position += m_BorderThickness / 2;
        }

        glm::vec4 color = m_Color;
        color.a *= alpha;
        Renderer2D::DrawQuad(position, size, color);
    }

}