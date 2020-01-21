#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"

#define VK_CHECK(x) SK_CORE_ASSERT((x) == VK_SUCCESS)

namespace Seek
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        SK_CORE_ERROR("Validation Layer: {0}", pCallbackData->pMessage);

        return VK_FALSE;
    }

    VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
    }

    void VulkanGraphicsContext::Init()
    {
        volkInitialize();

        CreateInstance();
        CreateDebugMessenger();
        PickPhysicalDevice();
        CreateLogicalDevice();

        uint32 numPhysicalDevices = 0;
        VK_CHECK(vkEnumeratePhysicalDevices(m_Instance, &numPhysicalDevices,
                                            nullptr));

        std::vector<VkPhysicalDevice> physicalDevices(numPhysicalDevices);
        VK_CHECK(vkEnumeratePhysicalDevices(m_Instance, &numPhysicalDevices,
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

        vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }

    void VulkanGraphicsContext::SwapBuffers() {}

    void VulkanGraphicsContext::CreateInstance()
    {
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

        VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance));

        volkLoadInstance(m_Instance);
    }

    void VulkanGraphicsContext::CreateDebugMessenger()
    {
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

        VK_CHECK(vkCreateDebugUtilsMessengerEXT(
            m_Instance, &messengerCreateInfo, nullptr, &m_DebugMessenger));
    }

    void VulkanGraphicsContext::PickPhysicalDevice() {}
    void VulkanGraphicsContext::CreateLogicalDevice() {}
}