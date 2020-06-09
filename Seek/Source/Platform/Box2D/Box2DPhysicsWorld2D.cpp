#include "SeekPCH.h"
#include "Platform/Box2D/Box2DPhysicsWorld2D.h"

#include "Seek/Debug/Instrumentor.h"

namespace Seek
{
    Box2DPhysicsWorld2D::Box2DPhysicsWorld2D()
    {
        SK_PROFILE_FUNCTION();

        b2Vec2 gravity(0.0f, 0.0f);
        m_Box2DWorld = CreateRef<b2World>(gravity);
    }

    Box2DPhysicsWorld2D::~Box2DPhysicsWorld2D() { SK_PROFILE_FUNCTION(); }

    void Box2DPhysicsWorld2D::OnUpdate(Timestep ts)
    {
        SK_PROFILE_FUNCTION();

        m_Box2DWorld->Step(ts, 6, 2);
    }
}
