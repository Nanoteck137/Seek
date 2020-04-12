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
        SK_CORE_ASSERT(font, "No font");
    }

    UIText::~UIText() {}

    void UIText::OnInit()
    {
        float height = m_Font->GetTextHeight(m_Text);
        m_Constraints->GetHeight()->SetPixelValue(height);
        CalculateScreenSpacePosition(true);
        UpdateObjects();
    }

    void UIText::OnUpdate(float deltaTime)
    {
        // NOTE(patrik): Debug bounds
        // Renderer2D::DrawQuad(GetPositionInPixels(), GetSizeInPixels(),
        //                     glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

        float32 alpha = m_TotalAlpha;
        float32 x = m_TextX * UIManager::GetDisplayWidth();
        float32 y = m_Position.y * UIManager::GetDisplayHeight();
        glm::vec4 color = m_Color;
        color.a *= alpha;

        Renderer2D::DrawText(glm::vec2(x, y), m_Text, m_Font, color, 1.0f);
    }

    void UIText::OnDimentionsChange() { UpdateObjects(); }

    void UIText::UpdateObjects()
    {
        float textWidth = m_Font->GetTextWidth(m_Text);

        switch (m_Alignment)
        {
            case TextAlignment::LEFT: m_TextX = 0.0f; break;
            case TextAlignment::CENTER:
                m_TextX = m_Position.x + m_Size.x / 2 -
                          (textWidth / 2) / UIManager::GetDisplayWidth();
                break;
            case TextAlignment::RIGHT:
                m_TextX = m_Position.x + m_Size.x -
                          (textWidth / UIManager::GetDisplayWidth());
                break;
        }
    }
}