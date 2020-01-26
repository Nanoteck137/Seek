#include "SeekPCH.h"
#include "Seek/Application.h"

#include "Seek/Timestep.h"

#include "Seek/Renderer/Renderer.h"
#include "Seek/Renderer/Renderer2D.h"

#include "Seek/Audio/AudioEngine.h"

#include "Seek/Debug/Instrumentor.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanRenderPass.h"
#include "Platform/Vulkan/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanPipelineLayout.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanCommandQueue.h"
#include "Platform/Vulkan/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanIndexBuffer.h"

#include "Seek/System/FileSystem.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <volk.h>

#include <vk_mem_alloc.h>

#include <spirv_reflect.hpp>

namespace Seek
{
    Application* Application::s_Instance = nullptr;

    struct VulkanObjects
    {
        VkDevice Device;
        VmaAllocator Allocator;
        VkQueue GraphicsQueue;
        VkQueue PresentQueue;

        VulkanRenderPass* RenderPass;
        std::vector<VulkanFramebuffer*> Framebuffers;

        VulkanPipelineLayout* PipelineLayout;
        VulkanGraphicsPipeline* Pipeline;

        VulkanVertexBuffer* VertexBuffer;
        VulkanIndexBuffer* IndexBuffer;

        VulkanCommandQueue* CommandQueue;
        VulkanCommandBuffer* CommandBuffer;
    };

    VulkanObjects obj;

    Application::Application()
    {
        SK_PROFILE_FUNCTION();

        SK_CORE_ASSERT(!s_Instance, "Application already exists");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(SK_BIND_EVENT_FN(Application::OnEvent));
        m_Window->SetVSync(false);

        Buffer buffer = FileSystem::ReadAllBuffer(
            "Assets/Shaders/Vulkan/triangle.vert.spv");

        spirv_cross::CompilerReflection compiler((const uint32*)buffer.Data,
                                                 buffer.Size / sizeof(uint32));

        spirv_cross::ShaderResources resources =
            compiler.get_shader_resources();

        for (auto& resource : resources.uniform_buffers)
        {
            unsigned set = compiler.get_decoration(
                resource.id, spv::DecorationDescriptorSet);
            unsigned binding =
                compiler.get_decoration(resource.id, spv::DecorationBinding);
            unsigned location =
                compiler.get_decoration(resource.id, spv::DecorationLocation);
            SK_CORE_TRACE("Uniform Buffers {0} at set = {1}, binding = {2}, "
                          "location = {3}\n",
                          resource.name.c_str(), set, binding, location);

            compiler.unset_decoration(resource.id,
                                      spv::DecorationDescriptorSet);

            compiler.set_decoration(resource.id, spv::DecorationBinding,
                                    set * 16 + binding);
        }

        /*spirv_cross::CompilerGLSL::Options options;
        options.version = 330;
        options.vulkan_semantics = true;
        options.emit_uniform_buffer_as_plain_uniforms = true;
        options.force_temporary = true;
        compiler.set_common_options(options);*/

        std::string source = compiler.compile();

        SK_CORE_INFO(source);

        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}};

        const std::vector<uint32> indices = {0, 1, 2, 2, 3, 0};

        VulkanGraphicsContext* context = dynamic_cast<VulkanGraphicsContext*>(
            m_Window->GetGraphicsContext());

        VkDevice device = context->GetDevice();
        VmaAllocator allocator = context->GetMemoryAllocator();
        VulkanSwapchain* swapchain = context->GetSwapchain();
        VulkanRenderPass* renderPass =
            new VulkanRenderPass(swapchain->GetFormat());

        std::vector<VulkanFramebuffer*> framebuffers(
            swapchain->GetImageCount());

        for (int i = 0; i < swapchain->GetImageCount(); i++)
        {
            VkImageView attachment = swapchain->GetImageViews()[i];

            framebuffers[i] = new VulkanFramebuffer(
                renderPass, attachment, swapchain->GetExtent().width,
                swapchain->GetExtent().height);
        }

        m_TriangleShader =
            Shader::Create("Assets/Shaders/Vulkan/triangle.vert.spv",
                           "Assets/Shaders/Vulkan/triangle.frag.spv");

        VulkanPipelineLayout* pipelineLayout = new VulkanPipelineLayout();
        VulkanGraphicsPipeline* pipeline = new VulkanGraphicsPipeline(
            renderPass,
            std::dynamic_pointer_cast<VulkanShader>(m_TriangleShader).get(),
            pipelineLayout);

        VulkanVertexBuffer* vertexBuffer = new VulkanVertexBuffer(
            vertices.data(), vertices.size() * sizeof(Vertex));

        VulkanIndexBuffer* indexBuffer =
            new VulkanIndexBuffer(indices.data(), indices.size());

        VulkanCommandQueue* commandQueue =
            new VulkanCommandQueue(context->GetGraphicsQueue());

        VulkanCommandBuffer* commandBuffer =
            new VulkanCommandBuffer(swapchain->GetImageCount());

        obj.Device = device;
        obj.Allocator = allocator;
        obj.GraphicsQueue = context->GetGraphicsQueue();
        obj.PresentQueue = context->GetPresentQueue();

        obj.RenderPass = renderPass;
        obj.Framebuffers = framebuffers;

        obj.PipelineLayout = pipelineLayout;
        obj.Pipeline = pipeline;

        obj.VertexBuffer = vertexBuffer;
        obj.IndexBuffer = indexBuffer;

        obj.CommandQueue = commandQueue;
        obj.CommandBuffer = commandBuffer;

        // m_ImGuiLayer = new ImGuiLayer();
        // PushOverlay(m_ImGuiLayer);

        // Renderer::Init();
        // AudioEngine::Init();

        m_Running = true;
    }

    Application::~Application()
    {
        vkDeviceWaitIdle(obj.Device);

        delete obj.CommandBuffer;
        delete obj.CommandQueue;

        delete obj.IndexBuffer;
        delete obj.VertexBuffer;

        delete obj.Pipeline;
        delete obj.PipelineLayout;

        for (VulkanFramebuffer* framebuffer : obj.Framebuffers)
            delete framebuffer;

        delete obj.RenderPass;

        m_Window.release();

        AudioEngine::Shutdown();
        Renderer::Shutdown();
    }

    void Application::OnEvent(Event& e)
    {
        SK_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(
            SK_BIND_EVENT_FN(Application::OnWindowClosed));

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

    void Application::Run()
    {
        SK_PROFILE_FUNCTION();

        float32 timer = 0;
        while (m_Running)
        {
            float32 time = (float32)glfwGetTime();
            Timestep timestep(time - m_LastFrameTime);
            m_LastFrameTime = time;

            timer += timestep;

            if (timer >= 1)
            {
                SK_CORE_TRACE("FPS: {0}", 1.0f / timestep);
                timer = 0;
            }

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            VulkanGraphicsContext* context = VulkanGraphicsContext::Get();
            VulkanSwapchain* swapchain = context->GetSwapchain();

            uint32 imageIndex = context->GetCurrentImage();

            // ---------------------------
            obj.CommandBuffer->Begin();

            obj.CommandBuffer->BeginRenderPass(obj.RenderPass,
                                               obj.Framebuffers[imageIndex]);

            obj.CommandBuffer->Clear();
            obj.CommandBuffer->BindPipeline(obj.Pipeline);

            obj.CommandBuffer->BindVertexBuffer(obj.VertexBuffer);
            obj.CommandBuffer->BindIndexBuffer(obj.IndexBuffer);

            obj.CommandBuffer->DrawIndexed(6, 0);

            obj.CommandBuffer->EndRenderPass();

            obj.CommandBuffer->End();
            // ---------------------------

            obj.CommandQueue->Submit(obj.CommandBuffer);

            if (m_ImGuiLayer)
            {
                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender(timestep);
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();
        }

        vkDeviceWaitIdle(obj.Device);
    }
}