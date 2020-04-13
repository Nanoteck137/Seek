#include "GameLayer.h"

GameLayer::GameLayer() : m_Camera(0.0f, 1280.0f, 0.0f, 720.0f)
{
    m_WorldController = new WorldController();
    m_MouseController = new MouseController(*m_WorldController);
}

void GameLayer::OnAttach() {}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate(Seek::Timestep ts)
{
    const float cameraMoveSpeed = 32.0f;

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

    Seek::RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
    Seek::RenderCommand::Clear();

    Seek::Renderer2D::BeginScene(m_Camera);

    m_WorldController->Update(ts);
    m_MouseController->Update(ts);

    Seek::Renderer2D::EndScene();
    Seek::Renderer2D::Flush();

    Seek::Window& window = Seek::Application::Get().GetWindow();
    uint32 width = window.GetWidth();
    uint32 height = window.GetHeight();

    cameraPos = m_Camera.GetPosition();
    glm::vec2 mouse = Seek::Input::GetMousePosition();
    glm::vec4 world =
        glm::vec4(mouse.x, mouse.y, 1.0f, 1.0f) * m_Camera.GetViewMatrix();
    // world += glm::vec2(cameraPos.x, cameraPos.y);

    // SK_APP_INFO("{}, {}", world.x, world.y);
}

void GameLayer::OnImGuiRender(Seek::Timestep ts) {}

void GameLayer::OnEvent(Seek::Event& event) {}
