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
        Renderer2D::DrawQuad(GetPositionInPixels(), GetSizeInPixels(), m_Color);
    }
}