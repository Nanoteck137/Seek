#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <volk.h>

namespace Seek
{
    class VulkanSwapchain
    {
    public:
        VulkanSwapchain(uint32 width, uint32 height);
        ~VulkanSwapchain();

        inline VkSwapchainKHR GetHandle() const { return m_Handle; }
        inline VkExtent2D GetExtent() const { return m_Extent; }
        inline VkFormat GetFormat() const { return m_Format; }

        inline uint32 GetImageCount() const { return m_ImageCount; }
        inline const std::vector<VkImage>& GetImages() { return m_Images; }
        inline const std::vector<VkImageView>& GetImageViews()
        {
            return m_ImageViews;
        }

    private:
        void Create(uint32 width, uint32 height);

    private:
        VkSwapchainKHR m_Handle = 0;
        VkExtent2D m_Extent = {};
        VkFormat m_Format = VK_FORMAT_UNDEFINED;

        uint32 m_ImageCount = 0;
        std::vector<VkImage> m_Images;
        std::vector<VkImageView> m_ImageViews;
    };
}