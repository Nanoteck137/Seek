#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <unordered_map>
#include "Modifier.h"
#include "Transition.h"

namespace Seek
{
    class UIComponent;

    class Animator
    {
    public:
        Animator(UIComponent* component);
        ~Animator();

        void ApplyModifier(const Ref<Transition>& transition, bool reverse,
                           float delay);
        bool IsDoingTransition(const Ref<Transition>& transition);

        void UpdateComponent();

        void Update(float deltaTime);

        void ApplyX(float xValue, bool change);
        void ApplyY(float yValue, bool change);
        void ApplyWidth(float width, bool change);
        void ApplyHeight(float height, bool change);
        void ApplyAlpha(float alpha, bool change);

    private:
        void ResetValues();

    public:
        inline float GetX() const { return m_XValue; }
        inline float GetY() const { return m_YValue; }
        inline float GetWidth() const { return m_Width; }
        inline float GetHeight() const { return m_Height; }
        inline float GetAlpha() const { return m_Alpha; }

    private:
        UIComponent* m_Component = nullptr;
        std::unordered_map<Ref<Transition>, Ref<Modifier>> m_Modifiers;

        bool m_PosChange = false;
        bool m_SizeChange = false;
        bool m_AlphaChange = false;

        float m_XValue = 0.0f;
        float m_YValue = 0.0f;
        float m_Width = 0.0f;
        float m_Height = 0.0f;
        float m_Alpha = 0.0f;
    };
}