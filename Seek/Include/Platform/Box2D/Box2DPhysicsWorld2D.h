#pragma once

#include "Seek/Physics/PhysicsWorld2D.h"

#include <box2d/box2d.h>

namespace Seek
{
    class Box2DPhysicsWorld2D : public PhysicsWorld2D
    {
    public:
        Box2DPhysicsWorld2D();
        ~Box2DPhysicsWorld2D();

        virtual void OnUpdate(Timestep ts) override;

        inline const Ref<b2World>& GetBox2DWorld() const
        {
            return m_Box2DWorld;
        }

    private:
        Ref<b2World> m_Box2DWorld;
    };
}