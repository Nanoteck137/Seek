#include "TestLayer.h"

TestLayer::TestLayer() : Layer("Test") {}

void TestLayer::OnAttach()
{
    m_Texture = Seek::Texture2D::Create("Assets/Textures/Test.png");

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

    m_Font =
        Seek::FontManager::CreateFont("Assets/Fonts/Roboto-Regular.ttf", 40.0f);
    m_Font2 =
        Seek::FontManager::CreateFont("Assets/Fonts/Roboto-Regular.ttf", 30.0f);

    Seek::UIContainer* container = Seek::UIManager::GetContainer();

    /*Seek::UIConstraints* constraints = new Seek::UIConstraints();
    constraints->SetX(new Seek::UICenterConstraint());
    constraints->SetY(new Seek::UICenterConstraint());
    constraints->SetWidth(new Seek::UIRatioConstraint(6.0f));
    constraints->SetHeight(new Seek::UIRelativeConstraint(0.09f));

    Seek::UIButton::Properties buttonProps;
    buttonProps.Label = "Hello World";
    buttonProps.Font = m_Font;
    buttonProps.LabelColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    buttonProps.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    buttonProps.HoverColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    buttonProps.ClickColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

    Seek::UIButton* button = new Seek::UIButton(buttonProps);
    button->SetAction([]() { SK_APP_INFO("Button clicked"); });

    container->Add(button, constraints);*/

    m_Menu = new Menu();
    m_Menu->SetHidden();
    Seek::UIConstraints* constraints = new Seek::UIConstraints();
    constraints->SetX(new Seek::UIRelativeConstraint(0.0f));
    constraints->SetY(new Seek::UIRelativeConstraint(0.0f));
    constraints->SetWidth(new Seek::UIRelativeConstraint(1.0f));
    constraints->SetHeight(new Seek::UIRelativeConstraint(1.0f));

    container->Add(m_Menu, constraints);

    // m_Sound = Seek::AudioEngine::CreateSound("Assets/Sounds/test.wav");
    // m_Sound->Play();
    // Sound* sound = Seek::AudioEngine::CreateSound();
    // effect->Play();
}

void TestLayer::OnDetach() {}

void TestLayer::OnUpdate(Seek::Timestep ts)
{
    m_World->OnUpdate(ts);

    const float cameraMoveSpeed = 5.0f;

    glm::vec2 cameraPos = m_Camera.GetPosition();
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

    Seek::UIManager::GetContainer()->Update(ts);

    /*Seek::Renderer2D::DrawQuad(m_Box->GetPosition(), m_Box->GetSize(),
                               {1.0f, 0.0f, 1.0f, 1.0f});

    Seek::Renderer2D::DrawQuad(m_Box2->GetPosition(), m_Box2->GetSize(),
                               {1.0f, 0.0f, 1.0f, 1.0f});

    Seek::Renderer2D::DrawQuad(m_GroundBox->GetPosition(),
                               m_GroundBox->GetSize(),
                               {1.0f, 0.0f, 1.0f, 1.0f});

    float x = Lerp(0.0f, 2.0f, m_Progress);
    Seek::Renderer2D::DrawQuad({-4 * 1.6f + 0.1f + x, -0.5f}, {2.0f, 4.0f},
                               {1.0f, 0.0f, 1.0f, 1.0f});*/

    /*String text = "Hello World";
    float32 width = m_Font->GetTextWidth(text);
    float32 height = m_Font->GetTextHeight(text);

    Seek::Renderer2D::DrawQuad(glm::vec2(32.0f, 200.0f),
                               glm::vec2(width, height),
                               glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

    Seek::Renderer2D::DrawText(glm::vec2(32.0f, 200.0f), text, m_Font);

    Seek::Renderer2D::DrawText(glm::vec2(32.0f, 70.0f), text, m_Font2,
    m_Scale);*/

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

void TestLayer::OnEvent(Seek::Event& event)
{
    Seek::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Seek::WindowResizeEvent>(
        SK_BIND_EVENT_FN(TestLayer::OnWindowResize));
    dispatcher.Dispatch<Seek::KeyReleasedEvent>(
        SK_BIND_EVENT_FN(TestLayer::OnKeyReleased));
}

bool TestLayer::OnWindowResize(Seek::WindowResizeEvent& event)
{
    m_Camera.Update();

    return false;
}

bool TestLayer::OnKeyReleased(Seek::KeyReleasedEvent& event)
{
    if (event.GetKeyCode() == SK_KEY_SPACE)
    {
        m_Menu->Display(!m_Menu->IsDisplayed());
    }
    return false;
}
