#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/App/Window.h"
#include "Seek/App/LayerStack.h"

#include "Seek/Events/ApplicationEvent.h"

#include "Seek/ImGui/ImGuiLayer.h"

namespace Seek
{
    class Application
    {
    protected:
        Application();

    public:
        virtual ~Application();

        void Run();
        void Close();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void OnEvent(Event& e);

        inline Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClosed(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        bool m_Running = false;
        float32 m_LastFrameTime = 0.0f;

        Scope<Window> m_Window = nullptr;
        ImGuiLayer* m_ImGuiLayer = nullptr;

        LayerStack m_LayerStack;

    public:
        inline static Application& Get() { return *s_Instance; }

    private:
        static Application* s_Instance;
    };
}