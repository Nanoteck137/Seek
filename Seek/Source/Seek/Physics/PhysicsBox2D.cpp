#include "SeekPCH.h"
#include "Seek/Physics/PhysicsBox2D.h"

#include "Platform/Box2D/Box2DPhysicsBox2D.h"

namespace Seek
{
    Ref<PhysicsBox2D> PhysicsBox2D::Create(const Ref<PhysicsWorld2D>& world,
                                           PhysicsBodyType type,
                                           const glm::vec2& position,
                                           const glm::vec2& size)
    {
        return CreateRef<Box2DPhysicsBox2D>(world, type, position, size);
    }
}