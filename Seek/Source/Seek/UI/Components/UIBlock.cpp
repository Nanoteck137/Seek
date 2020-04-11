#include "SeekPCH.h"
#include "Seek/UI/Components/UIBlock.h"

#include "Seek/UI/UIManager.h"

#include "Seek/Renderer/Renderer2D.h"

namespace Seek
{
    UIBlock::UIBlock() {}

    UIBlock::~UIBlock() {}

    void UIBlock::OnUpdate(float deltaTime)
    {
        glm::vec2 position = GetPositionInPixels();
        glm::vec2 size = GetSizeInPixels();

        if (m_Border)
        {
            Renderer2D::DrawQuad(position, size, m_BorderColor);

            size -= m_BorderThickness;
            position += m_BorderThickness / 2;
        }

        Renderer2D::DrawQuad(position, size, m_Color);
    }

}