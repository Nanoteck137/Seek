#include "SeekPCH.h"
#include "Seek/UI/Transition/Transition.h"

#undef max

namespace Seek
{
    Transition::Transition() {}

    Transition::~Transition()
    {
        for (auto& it : m_ValueDrivers)
        {
            delete it.second;
        }
    }

    Transition* Transition::Add(TransitionType* type,
                                ValueTransition* transition)
    {
        m_ValueDrivers[type] = transition;
        m_Duration = std::max(m_Duration, transition->GetDuration());
        return this;
    }

    Ref<Modifier> Transition::CreateModifier(const Ref<Modifier>& oldModifier,
                                             bool reverse, float delay)
    {
        float transitionDuration = m_Duration + delay;
        std::unordered_map<TransitionType*, Ref<ValueDriver>> driverInstances;

        for (auto& it : m_ValueDrivers)
        {
            Ref<ValueDriver> driverInstance =
                InitDriver(it.first, it.second, oldModifier, reverse, delay);
            driverInstances[it.first] = driverInstance;
        }

        return CreateRef<Modifier>(driverInstances, reverse,
                                   transitionDuration);
    }

    Ref<ValueDriver> Transition::InitDriver(TransitionType* type,
                                            ValueTransition* transition,
                                            const Ref<Modifier>& oldModifier,
                                            bool reverse, float delay)
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