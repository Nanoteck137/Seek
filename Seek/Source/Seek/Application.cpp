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

#include "Seek/System/FileSystem.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <volk.h>

#include <vk_mem_alloc.h>

#include <spirv_reflect.hpp>

namespace Seek
{
    Application* Application::s_Instance = nullptr;

    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;
    };

    struct VulkanBuffer
    {
        VkBuffer Handle;
        VmaAllocation Allocation;
    };

    struct VulkanObjects
    {
        VkDevice Device;
        VmaAllocator Allocator;
        VkQueue GraphicsQueue;
        VkQueue PresentQueue;

        VulkanRenderPass* RenderPass;
        std::vector<VulkanFramebuffer*> Framebuffers;

        VkPipelineLayout PipelineLayout;
        VkPipeline Pipeline;

        VulkanBuffer VertexBuffer;
        VulkanBuffer IndexBuffer;

        VkCommandPool CommandPool;
        VulkanCommandBuffer* CommandBuffer;
    };

    VulkanObjects obj;

    VulkanBuffer CreateBuffer(VmaAllocator allocator, VkBufferUsageFlags usage,
                              const void* data, uint32 size)
    {
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = usage;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VkBuffer buffer = 0;
        VmaAllocation alloc = 0;
        VK_CHECK(vmaCreateBuffer(allocator, &bufferCreateInfo, &allocInfo,
                                 &buffer, &alloc, nullptr));

        void* dataPtr;
        vmaMapMemory(allocator, alloc, &dataPtr);

        memcpy(dataPtr, data, size);

        vmaUnmapMemory(allocator, alloc);

        VulkanBuffer result = {};
        result.Handle = buffer;
        result.Allocation = alloc;

        return result;
    }

    Application::Application()
    {
        SK_PROFILE_FUNCTION();

        SK_CORE_ASSERT(!s_Instance, "Application already exists");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(SK_BIND_EVENT_FN(Application::OnEvent));
        m_Window->SetVSync(false);

        // std::vector<uint32_t> spirv_binary = load_spirv_file();

        Buffer buffer = FileSystem::ReadAllBuffer(
            "Assets/Shaders/Vulkan/triangle.vert.spv");

        spirv_cross::CompilerReflection compiler((const uint32*)buffer.Data,
                                                 buffer.Size / sizeof(uint32));

        // The SPIR-V is now parsed, and we can perform reflection on it.
        spirv_cross::ShaderResources resources =
            compiler.get_shader_resources();

        // Get all sampled images in the shader.
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

            // Modify the decoration to prepare it for GLSL.
            compiler.unset_decoration(resource.id,
                                      spv::DecorationDescriptorSet);

            // Some arbitrary remapping if we want.
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

        VkShaderModule vertexModule =
            std::dynamic_pointer_cast<VulkanShader>(m_TriangleShader)
                ->GetVertexShaderModule();
        VkShaderModule fragmentModule =
            std::dynamic_pointer_cast<VulkanShader>(m_TriangleShader)
                ->GetFragmentShaderModule();

        VkPipelineShaderStageCreateInfo vertexShaderStage = {};
        vertexShaderStage.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStage.module = vertexModule;
        vertexShaderStage.pName = "main";

        VkPipelineShaderStageCreateInfo fragmentShaderStage = {};
        fragmentShaderStage.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStage.module = fragmentModule;
        fragmentShaderStage.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStage,
                                                          fragmentShaderStage};

        VkVertexInputBindingDescription inputDesc = {};
        inputDesc.binding = 0;
        inputDesc.stride = sizeof(Vertex);
        inputDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        VkVertexInputAttributeDescription attributeDescriptions[2];
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &inputDesc;

        vertexInputInfo.vertexAttributeDescriptionCount = 2;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float32)swapchain->GetExtent().width;
        viewport.height = (float32)swapchain->GetExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = {0, 0};
        scissor.extent = swapchain->GetExtent();

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;

        VkPipelineLayout pipelineLayout = 0;
        VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo,
                                        nullptr, &pipelineLayout));

        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
        pipelineCreateInfo.sType =
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stageCount = 2;
        pipelineCreateInfo.pStages = shaderStages;
        pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
        pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
        pipelineCreateInfo.pViewportState = &viewportState;
        pipelineCreateInfo.pRasterizationState = &rasterizer;
        pipelineCreateInfo.pMultisampleState = &multisampling;
        pipelineCreateInfo.pDepthStencilState = nullptr;
        pipelineCreateInfo.pColorBlendState = &colorBlending;
        pipelineCreateInfo.pDynamicState = nullptr;
        pipelineCreateInfo.layout = pipelineLayout;
        pipelineCreateInfo.renderPass = renderPass->GetHandle();
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineCreateInfo.basePipelineIndex = -1;

        VkPipeline pipeline = 0;
        VK_CHECK(vkCreateGraphicsPipelines(device, 0, 1, &pipelineCreateInfo,
                                           nullptr, &pipeline));

        VulkanBuffer vertexBuffer =
            CreateBuffer(allocator, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         vertices.data(), vertices.size() * sizeof(Vertex));

        VulkanBuffer indexBuffer =
            CreateBuffer(allocator, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                         indices.data(), indices.size() * sizeof(uint32));

        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.flags =
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolCreateInfo.queueFamilyIndex =
            context->GetGraphicsFamilyIndex();

        VkCommandPool commandPool = 0;
        VK_CHECK(vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr,
                                     &commandPool));

        VulkanCommandBuffer* commandBuffer =
            new VulkanCommandBuffer(commandPool, swapchain->GetImageCount());

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

        obj.CommandPool = commandPool;
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

        vkDestroyCommandPool(obj.Device, obj.CommandPool, nullptr);

        vmaFreeMemory(obj.Allocator, obj.IndexBuffer.Allocation);
        vkDestroyBuffer(obj.Device, obj.IndexBuffer.Handle, nullptr);

        vmaFreeMemory(obj.Allocator, obj.VertexBuffer.Allocation);
        vkDestroyBuffer(obj.Device, obj.VertexBuffer.Handle, nullptr);

        vkDestroyPipeline(obj.Device, obj.Pipeline, nullptr);
        vkDestroyPipelineLayout(obj.Device, obj.PipelineLayout, nullptr);

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
            VkCommandBuffer commandBuffer =
                obj.CommandBuffer->GetCurrentHandle();

            VkRenderPassBeginInfo renderPassBeginInfo = {};
            renderPassBeginInfo.sType =
                VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = obj.RenderPass->GetHandle();
            renderPassBeginInfo.framebuffer =
                obj.Framebuffers[imageIndex]->GetHandle();
            renderPassBeginInfo.renderArea.extent = swapchain->GetExtent();
            renderPassBeginInfo.renderArea.offset = {0, 0};

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo,
                                 VK_SUBPASS_CONTENTS_INLINE);

            VkClearValue clearColor = {0.5f, 0.2f, 0.5f, 1.0f};

            VkClearAttachment attachment = {};
            attachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            attachment.colorAttachment = 0;
            attachment.clearValue = clearColor;

            VkClearRect clearRect = {};
            clearRect.rect.offset = {};
            clearRect.rect.extent = swapchain->GetExtent();
            clearRect.baseArrayLayer = 0;
            clearRect.layerCount = 1;

            vkCmdClearAttachments(commandBuffer, 1, &attachment, 1, &clearRect);

            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              obj.Pipeline);

            VkBuffer vertexBuffers[] = {obj.VertexBuffer.Handle};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(commandBuffer, obj.IndexBuffer.Handle, 0,
                                 VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            obj.CommandBuffer->End();
            // ---------------------------

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount = 0;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;
            submitInfo.signalSemaphoreCount = 0;

            VK_CHECK(vkQueueSubmit(obj.GraphicsQueue, 1, &submitInfo, 0));

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