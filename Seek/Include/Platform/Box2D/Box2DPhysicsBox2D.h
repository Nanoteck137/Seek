#pragma once

#include "Seek/Physics/PhysicsBox2D.h"

#include <box2d/box2d.h>

namespace Seek
{
    class Box2DPhysicsBox2D : public PhysicsBox2D
    {
    public:
        Box2DPhysicsBox2D(const Ref<PhysicsWorld2D>& world,
                          const glm::vec2& position, const glm::vec2& size);
        ~Box2DPhysicsBox2D();

        virtual const glm::vec2& GetPosition() override;
        virtual const glm::vec2& GetSize() const override { return m_Size; }

    private:
        void UpdatePosition();

    private:
        glm::vec2 m_Size;
        glm::vec2 m_Position;

        b2Body* m_Body;
        b2Fixture* m_Fixture;
    };
}