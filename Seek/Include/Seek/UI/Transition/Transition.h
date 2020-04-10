#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "TransitionType.h"
#include "ValueTransition.h"

#include "Modifier.h"

namespace Seek
{
    class Transition
    {
    public:
        Transition();
        ~Transition();

        Transition* Add(TransitionType* type, ValueTransition* transition);
        Ref<Modifier> CreateModifier(const Ref<Modifier>& oldModifier,
                                     bool reverse, float delay);
        Ref<ValueDriver> InitDriver(TransitionType* type,
                                    ValueTransition* transition,
                                    const Ref<Modifier>& oldModifier,
                                    bool reverse, float delay);

    private:
        std::unordered_map<TransitionType*, ValueTransition*> m_ValueDrivers;
        float m_Duration = 0.0f;
    };
}