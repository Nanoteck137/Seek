#include "SeekPCH.h"
#include "Seek/UI/Transition/Modifier.h"

#include "Seek/UI/Transition/Animator.h"

namespace Seek
{
    Modifier::Modifier(
        std::unordered_map<TransitionType*, ValueDriver*> valueDrivers,
        bool reverse, float duration)
        : m_ValueDrivers(valueDrivers), m_Reverse(reverse), m_Duration(duration)
    {
    }

    void Modifier::Update(float deltaTime, Animator* animator)
    {
        bool valueChanging = !HasFinishedTransition();
        m_Time += deltaTime;

        for (auto& it : m_ValueDrivers)
        {
            float value = it.second->Update(deltaTime);
            it.first->ApplyValue(animator, value, valueChanging);
        }
    }

    bool Modifier::HasFinishedTransition() { return m_Time > m_Duration; }

    float Modifier::GetCurrentValue(TransitionType* type)
    {
        ValueDriver* driver = m_ValueDrivers[type];
        return driver->GetCurrentValue();
    }

    bool Modifier::IsRedundant()
    {
        return m_Reverse && HasFinishedTransition();
    }
}