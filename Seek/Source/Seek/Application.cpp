#include "SeekPCH.h"
#include "Seek/Application.h"

#include "Seek/Log.h"

#include "Seek/Timestep.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Seek
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        SK_CORE_ASSERT(!s_Instance, "Application already exists");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(SK_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_VertexArray = VertexArray::Create();

        struct Vertex
        {
            glm::vec3 pos;
            glm::vec4 color;
        };

        Vertex vertices[] = {{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
                             {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                             {{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}};

        m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };

        m_VertexBuffer->SetLayout(layout);

        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32 indicies[3] = {0, 1, 2};

        m_IndexBuffer = IndexBuffer::Create(indicies, 3);
        m_VertexArray->AddIndexBuffer(m_IndexBuffer);

        String vertexShader = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

        String fragmentShader = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

        m_Shader = Shader::Create(vertexShader, fragmentShader);

        m_Running = true;
    }

    Application::~Application() {}

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

            timer += timestep.GetSeconds();
            if (timer >= 1)
            {
                SK_CORE_TRACE("Frame Info - Time: {0}ms Fps: {1}",
                              timestep.GetMilliseconds(),
                              1 / timestep.GetSeconds());
                timer = 0;
            }

            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();

            m_VertexArray->Bind();
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(),
                           GL_UNSIGNED_INT, nullptr);

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