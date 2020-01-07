#include "SeekPCH.h"
#include "Platform/Box2D/Box2DPhysicsWorld2D.h"

namespace Seek
{
    Box2DPhysicsWorld2D::Box2DPhysicsWorld2D()
    {
        b2Vec2 gravity(0.0f, -2.0f);
        m_Box2DWorld = CreateRef<b2World>(gravity);
    }

    Box2DPhysicsWorld2D::~Box2DPhysicsWorld2D() {}

    void Box2DPhysicsWorld2D::OnUpdate(Timestep ts)
    {
        m_Box2DWorld->Step(ts, 6, 2);
    }
}
