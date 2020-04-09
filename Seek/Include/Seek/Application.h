#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Window.h"

#include "Seek/Events/ApplicationEvent.h"
#include "Seek/LayerStack.h"

#include "Seek/ImGui/ImGuiLayer.h"

namespace Seek
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        static inline Application& Get() { return *s_Instance; }

        inline Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClosed(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        bool m_Running = false;

        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;

        LayerStack m_LayerStack;

        float32 m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;
    };
}