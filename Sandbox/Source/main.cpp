#include <Seek.h>
#include <imgui.h>
#include <box2d/box2d.h>

class TestLayer : public Seek::Layer
{
public:
    TestLayer() : Layer("Test"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {}

    void OnAttach() override
    {
        m_Texture = Seek::Texture2D::Create("Assets/Textures/Test.png");

        m_World = Seek::PhysicsWorld2D::Create();
        m_Box = Seek::PhysicsBox2D::Create(m_World, glm::vec2(0.0f, 3.0f),
                                           glm::vec2(1.0f, 1.0f));

        /*b2Vec2 gravity(0.0f, -1.0f);
        m_World = Seek::CreateRef<b2World>(gravity);

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -1.0f);
        groundBodyDef.type = b2_staticBody;

        m_GroundBody = m_World->CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(2.0f, 0.5f);

        m_GroundBody->CreateFixture(&groundBox, 0.0f);

        b2BodyDef boxBodyDef;
        boxBodyDef.position.Set(0.0f, 3.0f);
        boxBodyDef.type = b2_dynamicBody;

        m_BoxBody = m_World->CreateBody(&boxBodyDef);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(0.5f, 0.5f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        m_BoxBody->CreateFixture(&fixtureDef);*/
    }

    void OnDetach() override {}

    void OnUpdate(Seek::Timestep ts) override
    {
        m_World->OnUpdate(ts);

        const float cameraMoveSpeed = 5.0f;

        glm::vec3 cameraPos = m_Camera.GetPosition();
        float cameraRot = m_Camera.GetRotation();

        if (Seek::Input::IsKeyPressed(SK_KEY_A))
        {
            cameraPos.x -= cos(glm::radians(cameraRot)) * cameraMoveSpeed * ts;
            cameraPos.y -= sin(glm::radians(cameraRot)) * cameraMoveSpeed * ts;
        }
        else if (Seek::Input::IsKeyPressed(SK_KEY_D))
        {
            cameraPos.x += cos(glm::radians(cameraRot)) * cameraMoveSpeed * ts;
            cameraPos.y += sin(glm::radians(cameraRot)) * cameraMoveSpeed * ts;
        }

        if (Seek::Input::IsKeyPressed(SK_KEY_W))
        {
            cameraPos.x += -sin(glm::radians(cameraRot)) * cameraMoveSpeed * ts;
            cameraPos.y += cos(glm::radians(cameraRot)) * cameraMoveSpeed * ts;
        }
        else if (Seek::Input::IsKeyPressed(SK_KEY_S))
        {
            cameraPos.x -= -sin(glm::radians(cameraRot)) * cameraMoveSpeed * ts;
            cameraPos.y -= cos(glm::radians(cameraRot)) * cameraMoveSpeed * ts;
        }

        m_Camera.SetPosition(cameraPos);
        m_Camera.SetRotation(cameraRot);

        Seek::Renderer2D::ResetRenderStats();

        Seek::RenderCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
        Seek::RenderCommand::Clear();

        Seek::Renderer2D::BeginScene(m_Camera);

        /*const int NUM_SPRITES = 100;
        for (int y = 0; y < NUM_SPRITES; y++)
        {
            for (int x = 0; x < NUM_SPRITES; x++)
            {
                float xPos = (float)x;
                float yPos = (float)y;

                Seek::Renderer2D::DrawQuad(
                    glm::vec3(xPos * 1.05, yPos * 1.05, 0.0f), {1.0f, 1.0f},
                    glm::vec4(xPos / NUM_SPRITES, yPos / NUM_SPRITES, 0.0f,
                              1.0f));
            }
        }*/

        /*Ref<PhysicsWorld> world = PhysicsWorld::Create();

        Ref<PhysicsBox> box = PhysicsBox::Create(world, position, size);
        Ref<PhysicsBox> box = world->CreateBox(position, size);

        world->OnUpdate();*/

        // const b2Vec2& pos = m_BoxBody->GetPosition();
        const glm::vec2& pos = m_Box->GetPosition();
        Seek::Renderer2D::DrawQuad(glm::vec3(pos.x, pos.y, 0.0f), {1.0f, 1.0f},
                                   {1.0f, 0.0f, 1.0f, 1.0f});

        /*const b2Vec2& groundPos = m_GroundBody->GetPosition();
        Seek::Renderer2D::DrawQuad(glm::vec3(groundPos.x, groundPos.y, 0.0f),
                                   {4.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f});*/

        Seek::Renderer2D::EndScene();
        Seek::Renderer2D::Flush();

        // SK_APP_TRACE("X: {0} Y: {1}", pos.x, pos.y);
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Testing");
        ImGui::End();
    }

private:
    Seek::OrthographicCamera m_Camera;
    Seek::Ref<Seek::Texture2D> m_Texture;
    /*Seek::Ref<b2World> m_World;
    b2Body* m_GroundBody;
    b2Body* m_BoxBody;*/
    Seek::Ref<Seek::PhysicsWorld2D> m_World;
    Seek::Ref<Seek::PhysicsBox2D> m_Box;
};

class SandboxApp : public Seek::Application
{
public:
    SandboxApp() { PushLayer(new TestLayer()); }

    ~SandboxApp() {}
};

int main(int argc, char** argv)
{
    // TODO(patrik): Move this to a Engine Intiialize method or something like
    // that
    Seek::Log::Init();

    SandboxApp* app = new SandboxApp();
    app->Run();
    delete app;

    return 0;
}