#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Physics/PhysicsWorld2D.h"

#include <glm/glm.hpp>

namespace Seek
{
    enum class PhysicsBodyType
    {
        None = 0,
        Static,
        Dynamic,
    };

    class PhysicsBox2D
    {
    public:
        virtual ~PhysicsBox2D() = default;

        virtual void ApplyForceAtCenter(const glm::vec2& force) = 0;

        virtual PhysicsBodyType GetType() const = 0;
        virtual const glm::vec2& GetPosition() = 0;
        virtual const glm::vec2& GetSize() const = 0;

    public:
        static Ref<PhysicsBox2D> Create(const Ref<PhysicsWorld2D>& world,
                                        PhysicsBodyType type,
                                        const glm::vec2& position,
                                        const glm::vec2& size);
    };
}