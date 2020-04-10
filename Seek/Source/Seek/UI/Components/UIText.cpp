#include "SeekPCH.h"
#include "Seek/UI/Components/UIText.h"

#include "Seek/Renderer/Renderer2D.h"

#include "Seek/UI/UIConstraints.h"

#include "Seek/UI/UIManager.h"

namespace Seek
{
    UIText::UIText(const std::string& text, const Ref<Font>& font,
                   float fontSize, TextAlignment alignment)
        : m_Text(text), m_Font(font), m_FontSize(fontSize),
          m_Alignment(alignment)
    {
    }

    UIText::~UIText() {}

    void UIText::Init()
    {
        float height = m_Font->GetTextHeight(m_Text);
        m_Constraints->GetHeight()->SetPixelValue(height);
        CalculateScreenSpacePosition(true);
        UpdateObjects();
    }

    void UIText::UpdateSelf(float deltaTime)
    {
        Renderer2D::DrawQuad(GetPositionInPixels(), GetSizeInPixels(),
                             glm::vec4(0.8f, 0.0f, 0.8f, 1.0f));

        float x = m_TextX * UIManager::GetDisplayWidth();
        float y = m_Position.y * UIManager::GetDisplayHeight();
        Renderer2D::DrawText(glm::vec2(x, y), m_Text, m_Font, 1.0f);
    }

    void UIText::UpdateObjects()
    {
        float textWidth = m_Font->GetTextWidth(m_Text);

        switch (m_Alignment)
        {
            case TextAlignment::LEFT: m_TextX = 0.0f; break;
            case TextAlignment::CENTER:
                m_TextX = m_Size.x / 2 -
                          (textWidth / 2) / UIManager::GetDisplayWidth();
                break;
            case TextAlignment::RIGHT:
                m_TextX = m_Size.x - (textWidth / UIManager::GetDisplayWidth());
                break;
        }
    }
}