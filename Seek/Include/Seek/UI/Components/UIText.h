#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Renderer/Font.h"

#include "Seek/UI/UIComponent.h"

namespace Seek
{
    enum class TextAlignment
    {
        LEFT,
        CENTER,
        RIGHT
    };

    class UIText : public UIComponent
    {
    public:
        UIText(const String& text, const Ref<Font>& font, float32 fontSize,
               TextAlignment alignment);
        ~UIText();

        void SetText(const String& text)
        {
            m_Text = text;
            UpdateObjects();
        }
        inline const String& GetText() const { return m_Text; }

        void SetAlignment(TextAlignment alignment)
        {
            m_Alignment = alignment;
            UpdateObjects();
        }
        inline TextAlignment GetAlignment() const { return m_Alignment; }

        void SetFontSize(float32 size) { m_FontSize = size; }
        inline float32 GetFontSize() const { return m_FontSize; }

        inline const Ref<Font>& GetFont() const { return m_Font; }

        inline void SetColor(const glm::vec4& color) { m_Color = color; }
        inline const glm::vec4& GetColor() const { return m_Color; }

    protected:
        virtual void OnInit() override;
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnDimentionsChange() override;

    private:
        void UpdateObjects();

    public:
    private:
        std::string m_Text;
        TextAlignment m_Alignment = TextAlignment::LEFT;

        float32 m_FontSize = 0.0f;
        Ref<Font> m_Font = nullptr;

        float32 m_TextX = 0.0f;

        glm::vec4 m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    };
}