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

    private:
        float m_RoundedRadius = 0.0f;
    };
}
