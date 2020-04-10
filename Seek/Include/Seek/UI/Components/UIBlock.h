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

    private:
        float m_RoundedRadius = 0.0f;
        glm::vec4 m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    };
}
