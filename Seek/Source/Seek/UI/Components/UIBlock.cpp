#include "SeekPCH.h"
#include "Seek/UI/Components/UIBlock.h"

#include "Seek/UI/UIManager.h"

#include "Seek/Renderer/Renderer2D.h"

namespace Seek
{
    UIBlock::UIBlock() {}

    UIBlock::~UIBlock() {}

    void UIBlock::Init() {}

    void UIBlock::UpdateSelf(float deltaTime)
    {
        Renderer2D::DrawQuad(GetPositionInPixels(), GetSizeInPixels(),
                             glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    }
}