#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/UI/UIComponent.h"

namespace Seek
{
    class UIText : public UIComponent
    {
    public:
        UIText(const std::string& text, float fontSize);
        ~UIText();

    protected:
        virtual void Init() override;
        virtual void UpdateSelf(float deltaTime) override;

    private:
        void UpdateObjects();

    public:
        void SetText(const std::string& text)
        {
            m_Text = text;
            UpdateObjects();
        }
        inline const std::string& GetText() const { return m_Text; }

        void SetFontSize(float size)
        {
            m_FontSize = size;
            UpdateObjects();
        }
        inline float GetFontSize() const { return m_FontSize; }

    private:
        std::string m_Text;
        float m_FontSize;

        bool m_Update = false;
    };
}