#include "SeekPCH.h"
#include "Seek/Application.h"

#include "Seek/Timestep.h"

#include "Seek/Renderer/Renderer.h"
#include "Seek/Renderer/Renderer2D.h"

#include "Seek/Audio/AudioEngine.h"

#include "Seek/Debug/Instrumentor.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <volk.h>

namespace Seek
{
    Application* Application::s_Instance = nullptr;

#define VK_CHECK(x) SK_CORE_ASSERT((x) == VK_SUCCESS)

    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        SK_CORE_ERROR("Validation Layer: {0}", pCallbackData->pMessage);

        return VK_FALSE;
    }

    Application::Application()
    {
        SK_PROFILE_FUNCTION();

        SK_CORE_ASSERT(!s_Instance, "Application already exists");
        s_Instance = this;

        volkInitialize();

        uint32 propCount = 0;
        vkEnumerateInstanceLayerProperties(&propCount, nullptr);

        std::vector<VkLayerProperties> props(propCount);
        vkEnumerateInstanceLayerProperties(&propCount, props.data());

        std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};
        std::vector<const char*> extentions = {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "AppName";
        appInfo.applicationVersion = 0;
        appInfo.pEngineName = "Seek";
        appInfo.engineVersion = 0;
        appInfo.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledLayerCount = layers.size();
        instanceCreateInfo.ppEnabledLayerNames = layers.data();
        instanceCreateInfo.enabledExtensionCount = extentions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extentions.data();

        VkInstance instance = 0;
        VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));

        volkLoadInstance(instance);

        VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {};
        messengerCreateInfo.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        messengerCreateInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
        messengerCreateInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        messengerCreateInfo.pfnUserCallback = &VulkanDebugCallback;
        messengerCreateInfo.pUserData = nullptr; // Optional

        VkDebugUtilsMessengerEXT messenger = 0;
        VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance, &messengerCreateInfo,
                                                nullptr, &messenger));

        uint32 numPhysicalDevices = 0;
        VK_CHECK(
            vkEnumeratePhysicalDevices(instance, &numPhysicalDevices, nullptr));

        std::vector<VkPhysicalDevice> physicalDevices(numPhysicalDevices);
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &numPhysicalDevices,
                                            physicalDevices.data()));

        VkPhysicalDevice physicalDevice = physicalDevices[0];

        uint32 queueFamilyIndex = 0;

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        float priorities[] = {1.0f};

        VkDeviceQueueCreateInfo graphicsQueueInfo = {};
        graphicsQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        graphicsQueueInfo.queueFamilyIndex = queueFamilyIndex;
        graphicsQueueInfo.queueCount = 1;
        graphicsQueueInfo.pQueuePriorities = priorities;
        queueCreateInfos.push_back(graphicsQueueInfo);

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

        VkDevice device = 0;
        VK_CHECK(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr,
                                &device));

        VkQueue graphicsQueue = 0;
        vkGetDeviceQueue(device, queueFamilyIndex, 0, &graphicsQueue);

        vkDestroyDevice(device, nullptr);
        // vkDestroyDebugReportCallbackEXT(instance, debugReport, nullptr);
        vkDestroyDebugUtilsMessengerEXT(instance, messenger, nullptr);
        vkDestroyInstance(instance, nullptr);

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(SK_BIND_EVENT_FN(Application::OnEvent));
        m_Window->SetVSync(false);

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        Renderer::Init();
        AudioEngine::Init();

        m_Running = true;
    }

    Application::~Application()
    {
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
    }
}