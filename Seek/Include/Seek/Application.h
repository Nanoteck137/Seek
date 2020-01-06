#pragma once

#include "Seek/Window.h"

#include "Seek/Events/ApplicationEvent.h"
#include "Seek/LayerStack.h"

#include "Seek/ImGui/ImGuiLayer.h"

#include "Seek/Renderer/Buffer.h"
#include "Seek/Renderer/VertexArray.h"
#include "Seek/Renderer/Shader.h"
#include "Seek/Renderer/Texture.h"

#include "Seek/Renderer/OrthographicCamera.h"

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

    private:
        bool m_Running = false;

        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;

        LayerStack m_LayerStack;

        float32 m_LastFrameTime = 0.0f;

        Ref<Texture2D> m_Texture;

        OrthographicCamera m_Camera;

    private:
        static Application* s_Instance;
    };
}