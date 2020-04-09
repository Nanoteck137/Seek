#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "TransitionType.h"
#include "ValueDriver.h"

namespace Seek
{
    class Animator;

    class Modifier
    {
    public:
        Modifier(std::unordered_map<TransitionType*, ValueDriver*> valueDrivers,
                 bool reverse, float duration);

        void Update(float deltaTime, Animator* animator);

        bool HasFinishedTransition();
        float GetCurrentValue(TransitionType* type);
        bool IsRedundant();

    private:
        std::unordered_map<TransitionType*, ValueDriver*> m_ValueDrivers;
        float m_Duration = 0.0f;
        bool m_Reverse = false;

        float m_Time = 0.0f;
    };
}