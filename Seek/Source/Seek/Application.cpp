#include "SeekPCH.h"
#include "Seek/Application.h"

#include "Seek/Timestep.h"

#include "Seek/Renderer/Renderer.h"
#include "Seek/Renderer/Renderer2D.h"

#include "Seek/Audio/AudioEngine.h"

#include "Seek/Debug/Instrumentor.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Platform/Vulkan/VulkanShader.h"

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

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
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

        VkSwapchainKHR Swapchain;
        VkExtent2D SwapchainExtent;

        std::vector<VkImageView> SwapchainImageViews;
        VkRenderPass RenderPass;
        std::vector<VkFramebuffer> Framebuffers;

        VkPipelineLayout PipelineLayout;
        VkPipeline Pipeline;

        VulkanBuffer VertexBuffer;
        VulkanBuffer IndexBuffer;

        VkCommandPool CommandPool;
        std::vector<VkCommandBuffer> CommandBuffers;

        VkSemaphore ImageAvailableSemaphore;
        VkSemaphore RenderFinishedSemaphore;
    };

    VulkanObjects obj;

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device,
                                                  VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                                  &details.Capabilities);

        uint32 formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                             nullptr);

        if (formatCount != 0)
        {
            details.Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                                 details.Formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                                                  &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device, surface, &presentModeCount,
                details.PresentModes.data());
        }

        return details;
    }

    static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    static VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    static VkExtent2D
    ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32 width,
                     uint32 height)
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            VkExtent2D actualExtent = {width, height};

            actualExtent.width =
                std::max(capabilities.minImageExtent.width,
                         std::min(capabilities.maxImageExtent.width,
                                  actualExtent.width));
            actualExtent.height =
                std::max(capabilities.minImageExtent.height,
                         std::min(capabilities.maxImageExtent.height,
                                  actualExtent.height));

            return actualExtent;
        }
    }

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

        SwapChainSupportDetails swapchainSupport = QuerySwapChainSupport(
            context->GetPhysicalDevice(), context->GetSurface());

        VkSurfaceFormatKHR surfaceFormat =
            ChooseSwapSurfaceFormat(swapchainSupport.Formats);
        VkPresentModeKHR presentMode =
            ChooseSwapPresentMode(swapchainSupport.PresentModes);
        VkExtent2D extent =
            ChooseSwapExtent(swapchainSupport.Capabilities,
                             m_Window->GetWidth(), m_Window->GetHeight());

        uint32 imageCount = swapchainSupport.Capabilities.minImageCount + 1;

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = context->GetSurface();
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32 graphicsFamilyIndex = context->GetGraphicsFamilyIndex();
        uint32 presentFamilyIndex = context->GetPresentFamilyIndex();

        uint32_t queueFamilyIndices[] = {graphicsFamilyIndex,
                                         presentFamilyIndex};

        if (graphicsFamilyIndex != presentFamilyIndex)
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0;
            swapchainCreateInfo.pQueueFamilyIndices = 0;
        }

        swapchainCreateInfo.preTransform =
            swapchainSupport.Capabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.oldSwapchain = 0;

        VkSwapchainKHR swapchain = 0;
        VK_CHECK(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr,
                                      &swapchain));

        uint32 swapImageCount = 0;
        vkGetSwapchainImagesKHR(device, swapchain, &swapImageCount, nullptr);
        std::vector<VkImage> swapchainImages(swapImageCount);
        vkGetSwapchainImagesKHR(device, swapchain, &swapImageCount,
                                swapchainImages.data());

        std::vector<VkImageView> swapchainImageViews(swapchainImages.size());
        for (uint32 i = 0; i < swapchainImages.size(); i++)
        {
            VkImageViewCreateInfo imageViewCreateInfo = {};
            imageViewCreateInfo.sType =
                VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = swapchainImages[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = surfaceFormat.format;
            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.subresourceRange.aspectMask =
                VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            VK_CHECK(vkCreateImageView(device, &imageViewCreateInfo, nullptr,
                                       &swapchainImageViews[i]));
        }

        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = surfaceFormat.format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                   VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachment;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpass;
        renderPassCreateInfo.dependencyCount = 1;
        renderPassCreateInfo.pDependencies = &dependency;

        VkRenderPass renderPass = 0;
        VK_CHECK(vkCreateRenderPass(device, &renderPassCreateInfo, nullptr,
                                    &renderPass));

        std::vector<VkFramebuffer> framebuffers(swapchainImageViews.size());

        for (int i = 0; i < swapchainImageViews.size(); i++)
        {
            VkImageView attachments[] = {swapchainImageViews[i]};

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType =
                VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = renderPass;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.width = extent.width;
            framebufferCreateInfo.height = extent.height;
            framebufferCreateInfo.layers = 1;

            VkFramebuffer framebuffer = 0;
            VK_CHECK(vkCreateFramebuffer(device, &framebufferCreateInfo,
                                         nullptr, &framebuffer));

            framebuffers[i] = framebuffer;
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
        viewport.width = (float)extent.width;
        viewport.height = (float)extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = {0, 0};
        scissor.extent = extent;

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
        pipelineCreateInfo.pDepthStencilState = nullptr; // Optional
        pipelineCreateInfo.pColorBlendState = &colorBlending;
        pipelineCreateInfo.pDynamicState = nullptr; // Optional
        pipelineCreateInfo.layout = pipelineLayout;
        pipelineCreateInfo.renderPass = renderPass;
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineCreateInfo.basePipelineIndex = -1;              // Optional

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

        std::vector<VkCommandBuffer> commandBuffers(framebuffers.size());

        VkCommandBufferAllocateInfo commandBufferAllocInfo = {};
        commandBufferAllocInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocInfo.commandPool = commandPool;
        commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocInfo.commandBufferCount = framebuffers.size();

        VK_CHECK(vkAllocateCommandBuffers(device, &commandBufferAllocInfo,
                                          commandBuffers.data()));

        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore imageAvailableSemaphore = 0;
        VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr,
                                   &imageAvailableSemaphore));

        VkSemaphore renderFinishedSemaphore = 0;
        VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr,
                                   &renderFinishedSemaphore));

        obj.Device = device;
        obj.Allocator = allocator;
        obj.GraphicsQueue = context->GetGraphicsQueue();
        obj.PresentQueue = context->GetPresentQueue();
        obj.Swapchain = swapchain;
        obj.SwapchainExtent = extent;

        obj.SwapchainImageViews = swapchainImageViews;
        obj.RenderPass = renderPass;
        obj.Framebuffers = framebuffers;

        obj.PipelineLayout = pipelineLayout;
        obj.Pipeline = pipeline;

        obj.VertexBuffer = vertexBuffer;
        obj.IndexBuffer = indexBuffer;

        obj.CommandPool = commandPool;
        obj.CommandBuffers = commandBuffers;

        obj.ImageAvailableSemaphore = imageAvailableSemaphore;
        obj.RenderFinishedSemaphore = renderFinishedSemaphore;

        // m_ImGuiLayer = new ImGuiLayer();
        // PushOverlay(m_ImGuiLayer);

        // Renderer::Init();
        // AudioEngine::Init();

        m_Running = true;
    }

    Application::~Application()
    {
        vkDeviceWaitIdle(obj.Device);
        vkDestroySemaphore(obj.Device, obj.RenderFinishedSemaphore, nullptr);
        vkDestroySemaphore(obj.Device, obj.ImageAvailableSemaphore, nullptr);

        vkDestroyCommandPool(obj.Device, obj.CommandPool, nullptr);

        vmaFreeMemory(obj.Allocator, obj.IndexBuffer.Allocation);
        vkDestroyBuffer(obj.Device, obj.IndexBuffer.Handle, nullptr);

        vmaFreeMemory(obj.Allocator, obj.VertexBuffer.Allocation);
        vkDestroyBuffer(obj.Device, obj.VertexBuffer.Handle, nullptr);

        vkDestroyPipeline(obj.Device, obj.Pipeline, nullptr);
        vkDestroyPipelineLayout(obj.Device, obj.PipelineLayout, nullptr);

        for (VkFramebuffer framebuffer : obj.Framebuffers)
            vkDestroyFramebuffer(obj.Device, framebuffer, nullptr);

        vkDestroyRenderPass(obj.Device, obj.RenderPass, nullptr);

        for (VkImageView swapchainImageView : obj.SwapchainImageViews)
            vkDestroyImageView(obj.Device, swapchainImageView, nullptr);

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

            uint32 imageIndex;
            vkAcquireNextImageKHR(obj.Device, obj.Swapchain, UINT64_MAX,
                                  obj.ImageAvailableSemaphore, VK_NULL_HANDLE,
                                  &imageIndex);

            // ---------------------------
            VkCommandBuffer commandBuffer = obj.CommandBuffers[imageIndex];
            VkCommandBufferBeginInfo commandBufferBeginInfo = {};
            commandBufferBeginInfo.sType =
                VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            VK_CHECK(
                vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

            VkRenderPassBeginInfo renderPassBeginInfo = {};
            renderPassBeginInfo.sType =
                VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = obj.RenderPass;
            renderPassBeginInfo.framebuffer = obj.Framebuffers[imageIndex];
            renderPassBeginInfo.renderArea.extent = obj.SwapchainExtent;
            renderPassBeginInfo.renderArea.offset = {0, 0};

            VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo,
                                 VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              obj.Pipeline);

            VkBuffer vertexBuffers[] = {obj.VertexBuffer.Handle};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(commandBuffer, obj.IndexBuffer.Handle, 0,
                                 VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            VK_CHECK(vkEndCommandBuffer(commandBuffer));
            // ---------------------------

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            VkSemaphore waitSemaphores[] = {obj.ImageAvailableSemaphore};
            VkPipelineStageFlags waitStages[] = {
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;

            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            VkSemaphore signalSemaphores[] = {obj.RenderFinishedSemaphore};
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            VK_CHECK(vkQueueSubmit(obj.GraphicsQueue, 1, &submitInfo, 0));

            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;

            VkSwapchainKHR swapChains[] = {obj.Swapchain};
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr; // Optional

            vkQueuePresentKHR(obj.PresentQueue, &presentInfo);
            vkQueueWaitIdle(obj.PresentQueue);

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