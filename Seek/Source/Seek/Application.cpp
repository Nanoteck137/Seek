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
#include "Platform/Vulkan/VulkanUniformBuffer.h"

#include "Seek/System/FileSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <volk.h>
#include <vk_mem_alloc.h>
#include <spirv_reflect.hpp>

namespace Seek
{
    Application* Application::s_Instance = nullptr;

    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    struct VulkanObjects
    {
        VkDevice Device;
        VmaAllocator Allocator;
        VkQueue GraphicsQueue;
        VkQueue PresentQueue;

        VulkanPipelineLayout* PipelineLayout;
        VulkanGraphicsPipeline* Pipeline;

        VulkanVertexBuffer* VertexBuffer;
        VulkanIndexBuffer* IndexBuffer;
        VulkanUniformBuffer* UniformBuffer;

        VkDescriptorPool DescriptorPool;
        VkDescriptorSet DescriptorSet;

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

        VulkanGraphicsContext* context = dynamic_cast<VulkanGraphicsContext*>(
            m_Window->GetGraphicsContext());

        VkDevice device = context->GetDevice();
        VmaAllocator allocator = context->GetMemoryAllocator();
        VulkanSwapchain* swapchain = context->GetSwapchain();

        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        };

        const std::vector<uint32> indices = {0, 1, 2, 2, 3, 0};

        UniformBufferObject uniforms = {};
        uniforms.model = glm::mat4(1.0f);
        uniforms.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
        uniforms.proj = glm::perspective(
            glm::radians(45.0f),
            swapchain->GetExtent().width / (float)swapchain->GetExtent().height,
            0.1f, 10.0f);

        m_TriangleShader =
            Shader::Create("Assets/Shaders/Vulkan/triangle.vert.spv",
                           "Assets/Shaders/Vulkan/triangle.frag.spv");

        VulkanPipelineLayout* pipelineLayout = new VulkanPipelineLayout();
        VulkanGraphicsPipeline* pipeline = new VulkanGraphicsPipeline(
            context->GetRenderTarget()->GetRenderPass(),
            std::dynamic_pointer_cast<VulkanShader>(m_TriangleShader).get(),
            pipelineLayout);

        VulkanVertexBuffer* vertexBuffer = new VulkanVertexBuffer(
            vertices.data(), vertices.size() * sizeof(Vertex));

        VulkanIndexBuffer* indexBuffer =
            new VulkanIndexBuffer(indices.data(), indices.size());

        VulkanUniformBuffer* uniformBuffer =
            new VulkanUniformBuffer(&uniforms, sizeof(uniforms));

        VkDescriptorPoolSize poolSize = {};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = 1;

        VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
        descriptorPoolCreateInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCreateInfo.poolSizeCount = 1;
        descriptorPoolCreateInfo.pPoolSizes = &poolSize;
        descriptorPoolCreateInfo.maxSets = 1;

        VkDescriptorPool descriptorPool = 0;
        VK_CHECK(vkCreateDescriptorPool(device, &descriptorPoolCreateInfo,
                                        nullptr, &descriptorPool));

        VkDescriptorSetLayout descriptorSetLayout =
            pipelineLayout->GetDescriptorSetLayout();
        VkDescriptorSetAllocateInfo descriptorSetAllocInfo = {};
        descriptorSetAllocInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocInfo.descriptorPool = descriptorPool;
        descriptorSetAllocInfo.descriptorSetCount = 1;
        descriptorSetAllocInfo.pSetLayouts = &descriptorSetLayout;

        VkDescriptorSet descriptorSet = 0;
        VK_CHECK(vkAllocateDescriptorSets(device, &descriptorSetAllocInfo,
                                          &descriptorSet));

        VkDescriptorBufferInfo descriptorBufferInfo = {};
        descriptorBufferInfo.buffer = uniformBuffer->GetHandle();
        descriptorBufferInfo.offset = 0;
        descriptorBufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &descriptorBufferInfo;
        descriptorWrite.pImageInfo = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

        VulkanCommandQueue* commandQueue =
            new VulkanCommandQueue(context->GetGraphicsQueue());

        VulkanCommandBuffer* commandBuffer =
            new VulkanCommandBuffer(swapchain->GetImageCount());

        obj.Device = device;
        obj.Allocator = allocator;
        obj.GraphicsQueue = context->GetGraphicsQueue();
        obj.PresentQueue = context->GetPresentQueue();

        obj.PipelineLayout = pipelineLayout;
        obj.Pipeline = pipeline;

        obj.VertexBuffer = vertexBuffer;
        obj.IndexBuffer = indexBuffer;
        obj.UniformBuffer = uniformBuffer;

        obj.DescriptorPool = descriptorPool;
        obj.DescriptorSet = descriptorSet;

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

            // ---------------------------
            obj.CommandBuffer->Begin();

            obj.CommandBuffer->BeginRenderPass(context->GetRenderTarget());

            obj.CommandBuffer->Clear();
            obj.CommandBuffer->BindPipeline(obj.Pipeline);

            obj.CommandBuffer->BindVertexBuffer(obj.VertexBuffer);
            obj.CommandBuffer->BindIndexBuffer(obj.IndexBuffer);

            vkCmdBindDescriptorSets(obj.CommandBuffer->GetCurrentHandle(),
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    obj.PipelineLayout->GetHandle(), 0, 1,
                                    &obj.DescriptorSet, 0, nullptr);

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