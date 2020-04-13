#include "GameLayer.h"

GameLayer::GameLayer()
{
    m_WorldController = new WorldController();
    m_MouseController = new MouseController(m_Camera, *m_WorldController);
}

GameLayer::~GameLayer()
{
    if (m_MouseController)
    {
        delete m_MouseController;
        m_MouseController = nullptr;
    }

    if (m_WorldController)
    {
        delete m_WorldController;
        m_WorldController = nullptr;
    }
}

void GameLayer::OnAttach() {}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate(Seek::Timestep ts)
{
    const float32 cameraMoveSpeed = 4.0f;

    glm::vec2 cameraPos = m_Camera.GetPosition();
    float32 cameraRot = m_Camera.GetRotation();

    float32 size = m_Camera.GetOrthograpicSize();

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

    if (Seek::Input::IsKeyPressed(SK_KEY_E))
    {
        size -= 3 * ts;
    }
    else if (Seek::Input::IsKeyPressed(SK_KEY_Q))
    {
        size += 3 * ts;
    }

    m_Camera.SetPosition(cameraPos);
    m_Camera.SetRotation(cameraRot);
    m_Camera.SetOrthograpicSize(size);

    Seek::RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
    Seek::RenderCommand::Clear();

    Seek::Renderer2D::BeginScene(m_Camera);

    m_WorldController->Update(ts);
    m_MouseController->Update(ts);

    // Seek::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f),
    //                           glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

    Seek::Renderer2D::EndScene();
    Seek::Renderer2D::Flush();

    Seek::Window& window = Seek::Application::Get().GetWindow();
    uint32 width = window.GetWidth();
    uint32 height = window.GetHeight();

    float32 aspect = (float32)width / (float32)height;
    cameraPos = m_Camera.GetPosition();
    glm::vec2 mouse = Seek::Input::GetMousePosition();

    float32 camHeight = 2.0f * m_Camera.GetOrthograpicSize();
    float32 camWidth = camHeight * aspect;
    glm::vec2 world = glm::vec2(mouse.x * (camWidth / (float32)width),
                                mouse.y * (camHeight / (float32)height));
    // world += glm::vec2(cameraPos.x, cameraPos.y);

    // world.x /= world.w;
    // world.y /= world.w;

    // SK_APP_INFO("{}, {}", world.x, world.y);
}

void GameLayer::OnImGuiRender(Seek::Timestep ts) {}

void GameLayer::OnEvent(Seek::Event& event)
{
    m_MouseController->OnEvent(event);
}
