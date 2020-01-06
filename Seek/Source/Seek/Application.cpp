#include "SeekPCH.h"
#include "Seek/Application.h"

#include "Seek/Log.h"

#include "Seek/Timestep.h"

#include "Seek/Renderer/Renderer.h"

#include "Seek/Renderer/Renderer2D.h"

#include "Seek/Input.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Seek
{
    Application* Application::s_Instance = nullptr;

    Application::Application() : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        SK_CORE_ASSERT(!s_Instance, "Application already exists");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(SK_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        Renderer::Init();
        m_Texture = Texture2D::Create("Assets/Textures/test.png");

        m_Running = true;
    }

    Application::~Application() { Renderer2D::Shutdown(); }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(
            SK_BIND_EVENT_FN(Application::OnWindowClosed));

        // SK_CORE_TRACE("{0}", e);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    bool Application::OnWindowClosed(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::Run()
    {
        float32 timer = 0;
        while (m_Running)
        {
            float32 time = (float)glfwGetTime();
            Timestep timestep(time - m_LastFrameTime);
            m_LastFrameTime = time;

            timer += timestep;
            if (timer >= 1)
            {
                SK_CORE_TRACE("Frame Info - Time: {0}ms Fps: {1}",
                              timestep.GetMilliseconds(), 1.0f / timestep);
                timer = 0;
            }

            const float cameraMoveSpeed = 5.0f;

            glm::vec3 cameraPos = m_Camera.GetPosition();
            float cameraRot = m_Camera.GetRotation();

            if (Input::IsKeyPressed(SK_KEY_A))
            {
                cameraPos.x -=
                    cos(glm::radians(cameraRot)) * cameraMoveSpeed * timestep;
                cameraPos.y -=
                    sin(glm::radians(cameraRot)) * cameraMoveSpeed * timestep;
            }
            else if (Input::IsKeyPressed(SK_KEY_D))
            {
                cameraPos.x +=
                    cos(glm::radians(cameraRot)) * cameraMoveSpeed * timestep;
                cameraPos.y +=
                    sin(glm::radians(cameraRot)) * cameraMoveSpeed * timestep;
            }

            if (Input::IsKeyPressed(SK_KEY_W))
            {
                cameraPos.x +=
                    -sin(glm::radians(cameraRot)) * cameraMoveSpeed * timestep;
                cameraPos.y +=
                    cos(glm::radians(cameraRot)) * cameraMoveSpeed * timestep;
            }
            else if (Input::IsKeyPressed(SK_KEY_S))
            {
                cameraPos.x -=
                    -sin(glm::radians(cameraRot)) * cameraMoveSpeed * timestep;
                cameraPos.y -=
                    cos(glm::radians(cameraRot)) * cameraMoveSpeed * timestep;
            }

            m_Camera.SetPosition(cameraPos);
            m_Camera.SetRotation(cameraRot);

            Renderer2D::ResetRenderStats();

            RenderCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
            RenderCommand::Clear();

            Renderer2D::BeginScene(m_Camera);

            /*Renderer2D::DrawQuad({0.5f, 0.0f, -1.0f}, {1.0f, 1.0f},
                                 {0.2f, 0.2f, 0.8f, 0.5f});
            Renderer2D::DrawQuad({0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, m_Texture);*/

            /*Renderer2D::DrawQuad({0.0f, 0.0f, 0.0f}, {0.5f, 0.5f},
                                 {1.0f, 0.0f, 1.0f, 1.0f});*/
            /*Renderer2D::DrawQuad({1.0f, 0.0f, 0.0f}, {0.5f, 0.5f},
                                 {1.0f, 0.0f, 1.0f, 1.0f});*/

            const int NUM_SPRITES = 100;
            for (int y = 0; y < NUM_SPRITES; y++)
            {
                for (int x = 0; x < NUM_SPRITES; x++)
                {
                    float xPos = x;
                    float yPos = y;

                    Renderer2D::DrawQuad(
                        glm::vec3(xPos * 1.05, yPos * 1.05, 0.0f), {1.0f, 1.0f},
                        glm::vec4(xPos / NUM_SPRITES, yPos / NUM_SPRITES, 0.0f,
                                  1.0f));
                }
            }

            Renderer2D::EndScene();
            Renderer2D::Flush();

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }
}