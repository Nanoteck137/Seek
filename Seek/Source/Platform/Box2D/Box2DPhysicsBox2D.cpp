#include "SeekPCH.h"
#include "Platform/Box2D/Box2DPhysicsBox2D.h"

#include "Platform/Box2D/Box2DPhysicsWorld2D.h"

#include "Seek/Debug/Instrumentor.h"

namespace Seek
{
    static b2BodyType PhysicsBodyTypeToBox2D(PhysicsBodyType type)
    {
        switch (type)
        {
            case PhysicsBodyType::Static: return b2_staticBody;
            case PhysicsBodyType::Dynamic: return b2_dynamicBody;
        }

        SK_CORE_ASSERT(false, "Unknown type");
        return (b2BodyType)0;
    }

    Box2DPhysicsBox2D::Box2DPhysicsBox2D(const Ref<PhysicsWorld2D>& world,
                                         PhysicsBodyType type,
                                         const glm::vec2& position,
                                         const glm::vec2& size)
        : m_Type(type), m_Size(size), m_Position(position)
    {
        SK_PROFILE_FUNCTION();

        const Ref<b2World>& box2DWorld =
            std::dynamic_pointer_cast<Box2DPhysicsWorld2D>(world)
                ->GetBox2DWorld();

        b2BodyDef boxBodyDef;
        boxBodyDef.position.Set(position.x + (size.x / 2),
                                position.y + (size.y / 2));
        boxBodyDef.type = PhysicsBodyTypeToBox2D(type);

        m_Body = box2DWorld->CreateBody(&boxBodyDef);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(size.x / 2.0f, size.y / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        m_Fixture = m_Body->CreateFixture(&fixtureDef);
    }

    Box2DPhysicsBox2D::~Box2DPhysicsBox2D() {}

    void Box2DPhysicsBox2D::ApplyForceAtCenter(const glm::vec2& force)
    {
        SK_PROFILE_FUNCTION();

        m_Body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
    }

    const glm::vec2& Box2DPhysicsBox2D::GetPosition()
    {
        SK_PROFILE_FUNCTION();

        const b2Vec2& pos = m_Body->GetPosition();
        m_Position = {pos.x - (m_Size.x / 2), pos.y - (m_Size.y / 2)};
        return m_Position;
    }
}