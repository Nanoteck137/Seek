#include "SeekPCH.h"
#include "Seek/Physics/PhysicsWorld2D.h"

#include "Platform/Box2D/Box2DPhysicsWorld2D.h"

namespace Seek
{
    Ref<PhysicsWorld2D> PhysicsWorld2D::Create()
    {
        return CreateRef<Box2DPhysicsWorld2D>();
    }
}