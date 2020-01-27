#include "SeekPCH.h"
#include "Platform/Vulkan/VulkanRenderTargetSwapchain.h"

#include "Platform/Vulkan/VulkanGraphicsContext.h"

namespace Seek
{
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features)
    {
        for (VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(
                VulkanGraphicsContext::Get()->GetPhysicalDevice(), format,
                &props);

            if (tiling == VK_IMAGE_TILING_LINEAR &&
                (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                     (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        SK_CORE_ASSERT(false, "Failed to find supported format!")
    }

    static VkFormat FindDepthFormat()
    {
        return FindSupportedFormat(
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
             VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    VulkanRenderTargetSwapchain::VulkanRenderTargetSwapchain(
        VulkanSwapchain* swapchain)
        : m_Swapchain(swapchain)
    {
        CreateRenderPass();
        CreateDepthImage();
        CreateFramebuffers();
    }

    VulkanRenderTargetSwapchain::~VulkanRenderTargetSwapchain()
    {
        for (uint32 i = 0; i < m_Framebuffers.size(); i++)
        {
            vkDestroyFramebuffer(VulkanGraphicsContext::Get()->GetDevice(),
                                 m_Framebuffers[i], nullptr);
            m_Framebuffers[i] = 0;
        }

        if (m_RenderPass)
        {
            delete m_RenderPass;
            m_RenderPass = nullptr;
        }
    }

    VkFramebuffer VulkanRenderTargetSwapchain::GetCurrentFramebuffer() const
    {
        return m_Framebuffers[VulkanGraphicsContext::Get()->GetCurrentImage()];
    }

    void VulkanRenderTargetSwapchain::CreateRenderPass()
    {
        m_RenderPass =
            new VulkanRenderPass(m_Swapchain->GetFormat(), FindDepthFormat());
    }

    void VulkanRenderTargetSwapchain::CreateDepthImage()
    {
        VkFormat depthFormat = FindDepthFormat();
        VkImageTiling depthTiling = VK_IMAGE_TILING_OPTIMAL;

        VkImageCreateInfo depthImageCreateInfo = {};
        depthImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        depthImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;

        VkExtent2D extent = m_Swapchain->GetExtent();
        depthImageCreateInfo.extent.width = extent.width;
        depthImageCreateInfo.extent.height = extent.height;
        depthImageCreateInfo.extent.depth = 1;

        depthImageCreateInfo.mipLevels = 1;
        depthImageCreateInfo.arrayLayers = 1;
        depthImageCreateInfo.format = depthFormat;
        depthImageCreateInfo.tiling = depthTiling;
        depthImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthImageCreateInfo.usage =
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        depthImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        depthImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VK_CHECK(
            vmaCreateImage(VulkanGraphicsContext::Get()->GetMemoryAllocator(),
                           &depthImageCreateInfo, &allocInfo, &m_DepthImage,
                           &m_DepthMemory, nullptr));

        VkImageViewCreateInfo depthImageViewCreateInfo = {};
        depthImageViewCreateInfo.sType =
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        depthImageViewCreateInfo.image = m_DepthImage;
        depthImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        depthImageViewCreateInfo.format = depthFormat;
        depthImageViewCreateInfo.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_DEPTH_BIT;
        depthImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        depthImageViewCreateInfo.subresourceRange.levelCount = 1;
        depthImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        depthImageViewCreateInfo.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(VulkanGraphicsContext::Get()->GetDevice(),
                                   &depthImageViewCreateInfo, nullptr,
                                   &m_DepthImageView));
    }

    void VulkanRenderTargetSwapchain::CreateFramebuffers()
    {
        VkExtent2D extent = m_Swapchain->GetExtent();

        m_Framebuffers.resize(m_Swapchain->GetImageCount());
        for (uint32 i = 0; i < m_Swapchain->GetImageCount(); i++)
        {
            VkImageView swapchainImageView = m_Swapchain->GetImageViews()[i];
            std::array<VkImageView, 2> attachments = {swapchainImageView,
                                                      m_DepthImageView};

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType =
                VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = m_RenderPass->GetHandle();
            framebufferCreateInfo.attachmentCount = attachments.size();
            framebufferCreateInfo.pAttachments = attachments.data();
            framebufferCreateInfo.width = extent.width;
            framebufferCreateInfo.height = extent.height;
            framebufferCreateInfo.layers = 1;

            VkFramebuffer handle = 0;
            VK_CHECK(
                vkCreateFramebuffer(VulkanGraphicsContext::Get()->GetDevice(),
                                    &framebufferCreateInfo, nullptr, &handle));
            m_Framebuffers[i] = handle;
        }
    }
}