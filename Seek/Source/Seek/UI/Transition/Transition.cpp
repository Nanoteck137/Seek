#include "SeekPCH.h"
#include "Seek/UI/Transition/Transition.h"

#undef max

namespace Seek
{
    Transition::Transition() {}

    Transition::~Transition() {}

    Transition* Transition::Add(TransitionType* type,
                                ValueTransition* transition)
    {
        m_ValueDrivers[type] = transition;
        m_Duration = std::max(m_Duration, transition->GetDuration());
        return this;
    }

    Modifier* Transition::CreateModifier(Modifier* oldModifier, bool reverse,
                                         float delay)
    {
        float transitionDuration = m_Duration + delay;
        std::unordered_map<TransitionType*, ValueDriver*> driverInstances;

        for (auto& it : m_ValueDrivers)
        {
            ValueDriver* driverInstance =
                InitDriver(it.first, it.second, oldModifier, reverse, delay);
            driverInstances[it.first] = driverInstance;
        }

        return new Modifier(driverInstances, reverse, transitionDuration);
    }

    ValueDriver* Transition::InitDriver(TransitionType* type,
                                        ValueTransition* transition,
                                        Modifier* oldModifier, bool reverse,
                                        float delay)
    {
        float baseValue = type->GetBaseValue();
        float currentValue = reverse ? transition->GetHiddenValue() : baseValue;

        if (oldModifier)
        {
            currentValue = oldModifier->GetCurrentValue(type);
        }

        return transition->InitDriver(baseValue, currentValue, reverse, delay,
                                      m_Duration);
    }
}