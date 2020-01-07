#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Timestep.h"

namespace Seek
{
    class PhysicsWorld2D
    {
    public:
        virtual ~PhysicsWorld2D() = default;

        virtual void OnUpdate(Timestep ts) = 0;

    public:
        static Ref<PhysicsWorld2D> Create();
    };
}