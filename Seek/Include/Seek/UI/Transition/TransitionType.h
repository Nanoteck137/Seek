#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    class Animator;

    class TransitionType
    {
    public:
        static TransitionType* XPOS;
        static TransitionType* YPOS;
        static TransitionType* WIDTH;
        static TransitionType* HEIGHT;
        static TransitionType* ALPHA;

    private:
        TransitionType(float baseValue,
                       std::function<void(Animator*, float, bool)> setter);

    public:
        void ApplyValue(Animator* animator, float value, bool change);

    public:
        inline float GetBaseValue() const { return m_BaseValue; }

    private:
        float m_BaseValue;
        std::function<void(Animator*, float, bool)> m_Setter;
    };
}