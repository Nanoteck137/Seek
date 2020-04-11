#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/UI/UIComponent.h"

namespace Seek
{
    class UIBlock : public UIComponent
    {
    public:
        UIBlock();
        ~UIBlock();

    protected:
        virtual void OnUpdate(float deltaTime) override;

    public:
        inline void SetRoundedRadius(float radius) { m_RoundedRadius = radius; }
        inline float GetRoundedRadius() const { return m_RoundedRadius; }

        inline void SetColor(const glm::vec4& color) { m_Color = color; }
        inline const glm::vec4& GetColor() const { return m_Color; }

        inline void SetBorderActive(bool active) { m_Border = active; }
        inline bool GetBorderActive() const { return m_Border; }

        inline void SetBorderThickness(float thickness)
        {
            m_BorderThickness = thickness;
        }
        inline float GetBorderThickness() { return m_BorderThickness; }

        inline void SetBorderColor(const glm::vec4& color)
        {
            m_BorderColor = color;
        }
        inline const glm::vec4& GetBorderColor() const { return m_BorderColor; }

    private:
        float m_RoundedRadius = 0.0f;
        glm::vec4 m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        bool m_Border = false;
        float m_BorderThickness = 0.0f;
        glm::vec4 m_BorderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };
}
