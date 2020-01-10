#include "TestLayer.h"

TestLayer::TestLayer()
    : Layer("Test"), m_Camera(4 * -1.6f, 4 * 1.6f, 4 * -0.9f, 4 * 0.9f)
{
}

void TestLayer::OnAttach()
{
    m_Texture = Seek::Texture2D::Create("Assets/Textures/Test.png");

    bool res = Seek::FileSystem::FileExists("Assets/Textures/Test.png");

    m_World = Seek::PhysicsWorld2D::Create();
    m_Box = Seek::PhysicsBox2D::Create(m_World, Seek::PhysicsBodyType::Dynamic,
                                       glm::vec2(0.0f, 10.0f),
                                       glm::vec2(1.0f, 1.0f));
    m_Box2 = Seek::PhysicsBox2D::Create(m_World, Seek::PhysicsBodyType::Dynamic,
                                        glm::vec2(3.0f, 10.0f),
                                        glm::vec2(1.0f, 1.0f));

    m_GroundBox = Seek::PhysicsBox2D::Create(
        m_World, Seek::PhysicsBodyType::Static, glm::vec2(-25.0f, -4.0f),
        glm::vec2(50.0f, 1.0f));
}

void TestLayer::OnDetach() {}

void TestLayer::OnUpdate(Seek::Timestep ts)
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

    if (Seek::Input::IsKeyPressed(SK_KEY_H))
    {
        m_Box->ApplyForceAtCenter(glm::vec2(10.0f, 0.0f));
    }

    m_Camera.SetPosition(cameraPos);
    m_Camera.SetRotation(cameraRot);

    Seek::Renderer2D::ResetRenderStats();

    Seek::RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
    Seek::RenderCommand::Clear();

    Seek::Renderer2D::BeginScene(m_Camera);

    Seek::Renderer2D::DrawQuad(m_Box->GetPosition(), m_Box->GetSize(),
                               {1.0f, 0.0f, 1.0f, 1.0f});

    Seek::Renderer2D::DrawQuad(m_Box2->GetPosition(), m_Box2->GetSize(),
                               {1.0f, 0.0f, 1.0f, 1.0f});

    Seek::Renderer2D::DrawQuad(m_GroundBox->GetPosition(),
                               m_GroundBox->GetSize(),
                               {1.0f, 0.0f, 1.0f, 1.0f});

    Seek::Renderer2D::EndScene();
    Seek::Renderer2D::Flush();
}

void TestLayer::OnImGuiRender(Seek::Timestep ts)
{
    ImGui::Begin("Info");

    static uint32 fps = 0;
    static float frameTime = 0.0f;
    static float timer = 0.5f;

    if (timer >= 0.5f)
    {
        fps = (uint32)floor(1.0f / (float)ts);
        frameTime = ts.GetMilliseconds();
        timer = 0;
    }

    timer += ts;

    ImGui::Text("FPS: %u", fps);
    ImGui::Text("Framtime: %.2fms", frameTime);
    ImGui::End();
}