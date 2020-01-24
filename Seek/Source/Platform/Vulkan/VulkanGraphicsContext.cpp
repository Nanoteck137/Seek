#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"

#include <set>

// TODO(patrik): Remove this
#include <GLFW/glfw3.h>

namespace Seek
{
    VulkanGraphicsContext* VulkanGraphicsContext::s_Instance = nullptr;

    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        SK_CORE_ASSERT(false, "Validation Layer: {0}", pCallbackData->pMessage);

        return VK_FALSE;
    }

    String GetVulkanErrorString(VkResult errorCode)
    {
        switch (errorCode)
        {
#define STR(r)                                                                 \
    case VK_##r: return #r
            STR(NOT_READY);
            STR(TIMEOUT);
            STR(EVENT_SET);
            STR(EVENT_RESET);
            STR(INCOMPLETE);
            STR(ERROR_OUT_OF_HOST_MEMORY);
            STR(ERROR_OUT_OF_DEVICE_MEMORY);
            STR(ERROR_INITIALIZATION_FAILED);
            STR(ERROR_DEVICE_LOST);
            STR(ERROR_MEMORY_MAP_FAILED);
            STR(ERROR_LAYER_NOT_PRESENT);
            STR(ERROR_EXTENSION_NOT_PRESENT);
            STR(ERROR_FEATURE_NOT_PRESENT);
            STR(ERROR_INCOMPATIBLE_DRIVER);
            STR(ERROR_TOO_MANY_OBJECTS);
            STR(ERROR_FORMAT_NOT_SUPPORTED);
            STR(ERROR_SURFACE_LOST_KHR);
            STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
            STR(SUBOPTIMAL_KHR);
            STR(ERROR_OUT_OF_DATE_KHR);
            STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
            STR(ERROR_VALIDATION_FAILED_EXT);
            STR(ERROR_INVALID_SHADER_NV);
#undef STR
            default: return "UNKNOWN_ERROR";
        }
    }

    VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        SK_CORE_ASSERT(
            !s_Instance,
            "Multiple instances of a vulkan graphics context not allowed");

        s_Instance = this;
    }

    void VulkanGraphicsContext::Init()
    {
        volkInitialize();

        CreateInstance();
        CreateDebugMessenger();
        CreateSurface();

        PickPhysicalDevice();
        GetQueueFamiles();

        CreateLogicalDevice();
        GetQueues();

        CreateMemoryAllocator();

        int32 width;
        int32 height;
        glfwGetWindowSize(m_WindowHandle, &width, &height);
        m_Swapchain = new VulkanSwapchain(width, height);
        CreateSemaphores();

        GetNextIndex();
    }

    void VulkanGraphicsContext::Shutdown()
    {
        if (m_RenderFinishedSemaphore)
        {
            vkDestroySemaphore(m_Device, m_RenderFinishedSemaphore, nullptr);
            m_RenderFinishedSemaphore = 0;
        }

        if (m_ImageAvailableSemaphore)
        {
            vkDestroySemaphore(m_Device, m_ImageAvailableSemaphore, nullptr);
            m_ImageAvailableSemaphore = 0;
        }

        if (m_Swapchain)
        {
            delete m_Swapchain;
            m_Swapchain = nullptr;
        }

        if (m_MemoryAllocator)
        {
            vmaDestroyAllocator(m_MemoryAllocator);
            m_MemoryAllocator = 0;
        }

        if (m_Device)
        {
            vkDestroyDevice(m_Device, nullptr);
            m_Device = 0;
        }

        if (m_Surface)
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
            m_Surface = 0;
        }

        if (m_DebugMessenger)
        {
            vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger,
                                            nullptr);
            m_DebugMessenger = 0;
        }

        if (m_Instance)
        {
            vkDestroyInstance(m_Instance, nullptr);
            m_Instance = 0;
        }
    }

    void VulkanGraphicsContext::Present()
    {
        VkSemaphore signalSemaphores[1] = {m_RenderFinishedSemaphore};

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[1] = {m_Swapchain->GetHandle()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &m_CurrentImage;
        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(m_PresentQueue, &presentInfo);
        vkQueueWaitIdle(m_PresentQueue);

        GetNextIndex();
    }

    void VulkanGraphicsContext::CreateInstance()
    {
        uint32 propCount = 0;
        vkEnumerateInstanceLayerProperties(&propCount, nullptr);

        std::vector<VkLayerProperties> props(propCount);
        vkEnumerateInstanceLayerProperties(&propCount, props.data());

        std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};
        std::vector<const char*> extentions = {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

        uint32 glfwExtCount = 0;
        const char** glfwExts =
            glfwGetRequiredInstanceExtensions(&glfwExtCount);

        for (int i = 0; i < glfwExtCount; i++)
            extentions.push_back(glfwExts[i]);

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
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        messengerCreateInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        messengerCreateInfo.pfnUserCallback = &VulkanDebugCallback;
        messengerCreateInfo.pUserData = nullptr; // Optional

        VK_CHECK(vkCreateDebugUtilsMessengerEXT(
            m_Instance, &messengerCreateInfo, nullptr, &m_DebugMessenger));
    }

    void VulkanGraphicsContext::CreateSurface()
    {
        VK_CHECK(glfwCreateWindowSurface(m_Instance, m_WindowHandle, nullptr,
                                         &m_Surface));
    }

    void VulkanGraphicsContext::PickPhysicalDevice()
    {
        // TODO(patrik): More work here

        uint32 numPhysicalDevices = 0;
        VK_CHECK(vkEnumeratePhysicalDevices(m_Instance, &numPhysicalDevices,
                                            nullptr));

        std::vector<VkPhysicalDevice> physicalDevices(numPhysicalDevices);
        VK_CHECK(vkEnumeratePhysicalDevices(m_Instance, &numPhysicalDevices,
                                            physicalDevices.data()));

        for (VkPhysicalDevice physicalDevice : physicalDevices)
        {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(physicalDevice, &props);

            SK_CORE_INFO("GPU Detected: {0}", props.deviceName);
            SK_CORE_INFO("  {0}.{1}.{2}", VK_VERSION_MAJOR(props.driverVersion),
                         VK_VERSION_MINOR(props.driverVersion),
                         VK_VERSION_PATCH(props.driverVersion));
        }

        m_PhysicalDevice = physicalDevices[0];
    }

    void VulkanGraphicsContext::GetQueueFamiles()
    {
        uint32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice,
                                                 &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            m_PhysicalDevice, &queueFamilyCount, queueFamiles.data());

        for (int i = 0; i < queueFamiles.size(); i++)
        {
            if (queueFamiles[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_GraphicsQueueFamilyIndex = i;
            }

            VkBool32 supported = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, m_Surface,
                                                 &supported);
            if (supported == VK_TRUE)
            {
                m_PresentQueueFamilyIndex = i;
            }
        }
    }

    void VulkanGraphicsContext::CreateLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        float priorities[] = {1.0f};
        std::set<uint32> uniqueQueueFamilies = {m_GraphicsQueueFamilyIndex,
                                                m_PresentQueueFamilyIndex};

        for (uint32 queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueInfo = {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueFamilyIndex = queueFamily;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = priorities;
            queueCreateInfos.push_back(queueInfo);
        }

        std::vector<const char*> extentions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.enabledExtensionCount = extentions.size();
        deviceCreateInfo.ppEnabledExtensionNames = extentions.data();

        VK_CHECK(vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr,
                                &m_Device));
    }

    void VulkanGraphicsContext::GetQueues()
    {
        vkGetDeviceQueue(m_Device, m_GraphicsQueueFamilyIndex, 0,
                         &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, m_PresentQueueFamilyIndex, 0,
                         &m_PresentQueue);
    }

    void VulkanGraphicsContext::CreateMemoryAllocator()
    {
        VmaVulkanFunctions vulkanFunctions = {};
        vulkanFunctions.vkGetPhysicalDeviceProperties =
            vkGetPhysicalDeviceProperties;
        vulkanFunctions.vkGetPhysicalDeviceMemoryProperties =
            vkGetPhysicalDeviceMemoryProperties;
        vulkanFunctions.vkAllocateMemory = vkAllocateMemory;
        vulkanFunctions.vkFreeMemory = vkFreeMemory;
        vulkanFunctions.vkMapMemory = vkMapMemory;
        vulkanFunctions.vkUnmapMemory = vkUnmapMemory;
        vulkanFunctions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
        vulkanFunctions.vkInvalidateMappedMemoryRanges =
            vkInvalidateMappedMemoryRanges;
        vulkanFunctions.vkBindBufferMemory = vkBindBufferMemory;
        vulkanFunctions.vkBindImageMemory = vkBindImageMemory;
        vulkanFunctions.vkGetBufferMemoryRequirements =
            vkGetBufferMemoryRequirements;
        vulkanFunctions.vkGetImageMemoryRequirements =
            vkGetImageMemoryRequirements;
        vulkanFunctions.vkCreateBuffer = vkCreateBuffer;
        vulkanFunctions.vkDestroyBuffer = vkDestroyBuffer;
        vulkanFunctions.vkCreateImage = vkCreateImage;
        vulkanFunctions.vkDestroyImage = vkDestroyImage;
        vulkanFunctions.vkCmdCopyBuffer = vkCmdCopyBuffer;

        VmaAllocatorCreateInfo allocatorCreateInfo = {};
        allocatorCreateInfo.physicalDevice = m_PhysicalDevice;
        allocatorCreateInfo.device = m_Device;
        allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

        VK_CHECK(vmaCreateAllocator(&allocatorCreateInfo, &m_MemoryAllocator));
    }

    void VulkanGraphicsContext::CreateSemaphores()
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VK_CHECK(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr,
                                   &m_ImageAvailableSemaphore));
        VK_CHECK(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr,
                                   &m_RenderFinishedSemaphore));
    }

    void VulkanGraphicsContext::GetNextIndex()
    {
        vkAcquireNextImageKHR(m_Device, m_Swapchain->GetHandle(), UINT64_MAX,
                              m_ImageAvailableSemaphore, VK_NULL_HANDLE,
                              &m_CurrentImage);
    }

}