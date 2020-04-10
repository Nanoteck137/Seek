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
        UIText(const std::string& text, const Ref<Font>& font, float fontSize,
               TextAlignment alignment);
        ~UIText();

    protected:
        virtual void OnInit() override;
        virtual void OnUpdate(float deltaTime) override;
        virtual void OnDimentionsChange() override;

    private:
        void UpdateObjects();

    public:
        void SetText(const std::string& text)
        {
            m_Text = text;
            UpdateObjects();
        }
        inline const std::string& GetText() const { return m_Text; }

        void SetAlignment(TextAlignment alignment)
        {
            m_Alignment = alignment;
            UpdateObjects();
        }
        inline TextAlignment GetAlignment() const { return m_Alignment; }

        void SetFontSize(float size) { m_FontSize = size; }
        inline float GetFontSize() const { return m_FontSize; }

        inline const Ref<Font>& GetFont() const { return m_Font; }

    private:
        std::string m_Text;
        TextAlignment m_Alignment = TextAlignment::LEFT;

        float32 m_FontSize = 0.0f;
        Ref<Font> m_Font = nullptr;

        bool m_Update = false;

        float m_TextX = 0.0f;
    };
}