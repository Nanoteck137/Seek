#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

#undef min
#undef max

namespace Seek
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    static SwapChainSupportDetails
    QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
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

    VulkanSwapchain::VulkanSwapchain(uint32 width, uint32 height)
    {
        Create(width, height);
    }

    VulkanSwapchain::~VulkanSwapchain()
    {
        for (VkImageView imageView : m_ImageViews)
        {
            vkDestroyImageView(VulkanGraphicsContext::Get()->GetDevice(),
                               imageView, nullptr);
        }

        vkDestroySwapchainKHR(VulkanGraphicsContext::Get()->GetDevice(),
                              m_Handle, nullptr);
    }

    void VulkanSwapchain::Create(uint32 width, uint32 height)
    {
        VkDevice device = VulkanGraphicsContext::Get()->GetDevice();

        SwapChainSupportDetails swapchainSupport = QuerySwapChainSupport(
            VulkanGraphicsContext::Get()->GetPhysicalDevice(),
            VulkanGraphicsContext::Get()->GetSurface());

        VkSurfaceFormatKHR surfaceFormat =
            ChooseSwapSurfaceFormat(swapchainSupport.Formats);
        VkPresentModeKHR presentMode =
            ChooseSwapPresentMode(swapchainSupport.PresentModes);
        VkExtent2D extent =
            ChooseSwapExtent(swapchainSupport.Capabilities, width, height);

        m_Extent = extent;
        m_Format = surfaceFormat.format;

        uint32 imageCount = swapchainSupport.Capabilities.minImageCount + 1;

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface =
            VulkanGraphicsContext::Get()->GetSurface();
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = m_Format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32 graphicsFamilyIndex =
            VulkanGraphicsContext::Get()->GetGraphicsFamilyIndex();
        uint32 presentFamilyIndex =
            VulkanGraphicsContext::Get()->GetPresentFamilyIndex();

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

        VK_CHECK(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr,
                                      &m_Handle));

        VK_CHECK(
            vkGetSwapchainImagesKHR(device, m_Handle, &m_ImageCount, nullptr));
        std::vector<VkImage> images(m_ImageCount);
        VK_CHECK(vkGetSwapchainImagesKHR(device, m_Handle, &m_ImageCount,
                                         images.data()));

        std::vector<VkImageView> imageViews(images.size());
        for (uint32 i = 0; i < images.size(); i++)
        {
            VkImageViewCreateInfo imageViewCreateInfo = {};
            imageViewCreateInfo.sType =
                VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = images[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = m_Format;
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
                                       &imageViews[i]));

            m_Images = images;
            m_ImageViews = imageViews;
        }
    }

}