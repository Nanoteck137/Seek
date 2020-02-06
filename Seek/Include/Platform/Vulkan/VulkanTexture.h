#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <volk.h>
#include <vk_mem_alloc.h>

namespace Seek
{
    class VulkanTexture
    {
    public:
        VulkanTexture(const String& path);
        ~VulkanTexture();

        inline uint32 GetWidth() const { return m_Width; }
        inline uint32 GetHeight() const { return m_Height; }

        inline VkImage GetImageHandle() const { return m_Image; }
        inline VkImageView GetImageView() const { return m_ImageView; }
        inline VkSampler GetSampler() const { return m_Sampler; }

    private:
        uint32 m_Width = 0;
        uint32 m_Height = 0;

        VkImage m_Image = 0;
        VmaAllocation m_ImageMemory = 0;

        VkImageView m_ImageView = 0;
        VkSampler m_Sampler = 0;
    };
}