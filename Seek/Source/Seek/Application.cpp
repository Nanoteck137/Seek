#include "SeekPCH.h"
#include "Seek/App/Application.h"

// TODO(patrik): Remove these includes
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Seek/App/Timestep.h"

#include "Seek/Audio/AudioEngine.h"

#include "Seek/Debug/Instrumentor.h"

#include "Seek/Renderer/Renderer.h"
#include "Seek/Renderer/Renderer2D.h"
#include "Seek/Renderer/FontManager.h"

#include "Seek/UI/UIManager.h"

namespace Seek
{
    // Static Variables initializtion
    Application* Application::s_Instance = nullptr;

    // NOTE: Constructor
    Application::Application()
    {
        SK_PROFILE_FUNCTION();

        SK_CORE_ASSERT(!s_Instance, "Application already exists");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(SK_BIND_EVENT_FN(Application::OnEvent));
        m_Window->SetVSync(false);

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        // TODO(patrik): Move this to an engine initialize method
        Renderer::Init();
        AudioEngine::Init();
        FontManager::Init();
        UIManager::Init();

        UIManager::UpdateDisplaySize(m_Window->GetWidth(),
                                     m_Window->GetHeight());

        m_Running = true;
    }

    Application::~Application()
    {
        AudioEngine::Shutdown();
        Renderer::Shutdown();
        FontManager::Shutdown();
        UIManager::Shutdown();
    }

    // NOTE: Virtual Methods

    // NOTE: Normal Methods
    void Application::Run()
    {
        SK_PROFILE_FUNCTION();

        float32 timer = 0;
        while (m_Running)
        {
            float32 time = (float)glfwGetTime();
            Timestep timestep(time - m_LastFrameTime);
            m_LastFrameTime = time;

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender(timestep);
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }

        glfwTerminate();
    }

    void Application::Close() { m_Running = false; }

    void Application::PushLayer(Layer* layer)
    {
        SK_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        SK_PROFILE_FUNCTION();

        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        SK_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(
            SK_BIND_EVENT_FN(Application::OnWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(
            SK_BIND_EVENT_FN(Application::OnWindowResize));

        UIManager::GetContainer()->OnEvent(e);

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

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        RenderCommand::UpdateDisplaySize(e.GetWidth(), e.GetHeight());
        UIManager::UpdateDisplaySize(e.GetWidth(), e.GetHeight());

        return false;
    }

    // NOTE: Static Methods
}