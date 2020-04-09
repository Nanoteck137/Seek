#include "SeekPCH.h"
#include "Seek/UI/Components/UIText.h"

#include "Seek/UI/UIConstraints.h"

namespace Seek
{
    UIText::UIText(const std::string& text, float fontSize)
        : m_Text(text), m_FontSize(fontSize)
    {
        UpdateObjects();
    }

    UIText::~UIText() {}

    void UIText::Init()
    {
        // float height = m_RenderingText.getLocalBounds().height;
        // m_Constraints->GetHeight()->SetPixelValue(m_FontSize);
        // CalculateScreenSpacePosition(true);
    }

    void UIText::UpdateSelf(float deltaTime) {}

    void UIText::UpdateObjects() {}
}