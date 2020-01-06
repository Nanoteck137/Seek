#include <Seek.h>
#include <imgui.h>

class TestLayer : public Seek::Layer
{
public:
    TestLayer() : Layer("Test"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {}

    void OnAttach() override
    {
        m_Texture = Seek::Texture2D::Create("Assets/Textures/Test.png");
    }

    void OnDetach() override {}

    void OnUpdate(Seek::Timestep ts) override
    {
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

        const int NUM_SPRITES = 100;
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
        }

        Seek::Renderer2D::EndScene();
        Seek::Renderer2D::Flush();
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